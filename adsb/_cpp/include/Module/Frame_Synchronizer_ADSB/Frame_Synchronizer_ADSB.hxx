/*!
 * \file
 * \brief Sens or receive samples to or from a Radio
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef FRAME_SYNCHRONIZER_ADSB_HXX_
#define FRAME_SYNCHRONIZER_ADSB_HXX_

#include <sstream>
#include <complex>
#include <csignal>
#include <algorithm>

#include "Module/Frame_Synchronizer_ADSB/Frame_Synchronizer_ADSB.hpp"

namespace aff3ct
{
namespace module
{

template <typename R>
Frame_Synchronizer_ADSB<R>::Frame_Synchronizer_ADSB(const int F, const R threshold)
: Stateful(),
  F(F),
  threshold(threshold),
  old_frames(F/2, std::vector<R>(120*2*2, (R)0)), // 120*(2 for complex data)*(2 for oversampling by 2)
  old_pos(0),
  old_path(0),
  old_sync_val(0)
{
	const std::string name = "Frame_Synchronizer_ADSB";
	this->set_name(name);
	this->set_short_name(name);

	if (F <= 0)
	{
		std::stringstream message;
		message << "'F' has to be greater than 0 ('F' = " << F << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
    
	auto &p1 = this->create_task("synchronize");
    auto F2 = F/2; // to capture by value in lambda

    std::vector<size_t> p1s_data(F2,0);
    for (int f = 0; f < F2; f++)
        p1s_data[f] = this->template create_socket_in<R>(p1, "data" + std::to_string(f), 120*2*2); // 120*(2 for complex data)*(2 for oversampling by 2)

    std::vector<size_t> p1s_num(F2,0);
    for (int f = 0; f < F2; f++)
        p1s_num[f] = this->template create_socket_in<R>(p1, "num" + std::to_string(f), 120*2); // 120*(2 for oversampling by 2)

    std::vector<size_t> p1s_denom(F2,0);
    for (int f = 0; f < F2; f++)
        p1s_denom[f] = this->template create_socket_in<R>(p1, "denom" + std::to_string(f), 120*2); // 120*(2 for oversampling by 2)

    auto p1s_out = this->template create_socket_out<R>(p1, "out", 112*2*2); // 112*(2 for complex data)*(2 for oversampling by 2)
    auto p1s_sync_val = this->template create_socket_out<R>(p1, "sync_val", 1);
    auto p1s_pos = this->template create_socket_out<int32_t>(p1, "pos", 1);
    auto p1s_path = this->template create_socket_out<int32_t>(p1, "path", 1);

    this->create_codelet(p1, [p1s_data, p1s_num, p1s_denom, p1s_out, p1s_sync_val, p1s_pos, p1s_path, F2](spu::module::Module &m, spu::runtime::Task &t, const size_t frame_id) -> int
    {
        auto& mod = static_cast<Frame_Synchronizer_ADSB<R>&>(m);

        std::vector<const R*> data(F2, nullptr);
        std::vector<const R*> num(F2, nullptr);
        std::vector<const R*> denom(F2, nullptr);

        for (int f = 0; f < F2; f++)
        {
            data[f] = static_cast<const R*>(t[p1s_data[f]].get_dataptr());
            num[f] = static_cast<const R*>(t[p1s_num[f]].get_dataptr());
            denom[f] = static_cast<const R*>(t[p1s_denom[f]].get_dataptr());
        }

        auto out = static_cast<R*>(t[p1s_out].get_dataptr());
        auto sync_val = static_cast<R*>(t[p1s_sync_val].get_dataptr());
        auto pos = static_cast<int32_t*>(t[p1s_pos].get_dataptr());
        auto path = static_cast<int32_t*>(t[p1s_path].get_dataptr());
        // Call the internal synchronize function
        mod._synchronize(data, num, denom, out, sync_val, pos, path, frame_id);
        return spu::runtime::status_t::SUCCESS;
    });
}

template<typename R>
void
Frame_Synchronizer_ADSB<R>::_synchronize(std::vector<const R*> data, std::vector<const R*> num, std::vector<const R*> denom, R* out, R* sync_val, int32_t* pos, int32_t* path, const int frame_id)
{   
    R max_sync_val = (R)0;
    int new_pos = 0;
    int new_path = 0;

    for (int f = 0; f < F/2; f++)
    {
        for (int i = 0; i < 120*2; i++) // 120*2 for oversampling by 2
        {
            R current_sync_val = (R)0;
            if (denom[f][i] > 0)
                current_sync_val = num[f][i]/denom[f][i];

            if (max_sync_val < current_sync_val)
            {
                max_sync_val = current_sync_val;
                new_pos = i;
                new_path = f;
            }
        }
    }
    int frame_size_no_preamble = 112*2*2; // 112*(2 for complex data)*(2 for oversampling by 2)
    int frame_size = 120*2*2; // 120*(2 for complex data)*(2 for oversampling by 2)
    bool output_frame = old_sync_val >= threshold;
    if (output_frame)
    {
        int real_samples_to_copy_in_old_frame = 0;
        int real_samples_to_copy_in_current_frame = 0;
        int real_samples_after_start = frame_size - (old_pos+1)*2; // copy size for old_frame, 2 for complex data
        if (real_samples_after_start > frame_size_no_preamble)
        {
            real_samples_to_copy_in_old_frame = frame_size_no_preamble;
        }
        else
        {
            real_samples_to_copy_in_old_frame = real_samples_after_start;
            real_samples_to_copy_in_current_frame = frame_size_no_preamble - real_samples_after_start;
        }
        std::copy(old_frames[old_path].begin() + (old_pos+1)*2, old_frames[old_path].begin() + (old_pos+1)*2 + real_samples_to_copy_in_old_frame, out);
        std::copy(data[old_path], data[old_path] + real_samples_to_copy_in_current_frame, out + real_samples_to_copy_in_old_frame);

        *sync_val = old_sync_val;
        *pos = old_pos;
        *path = old_path;
    }
    for (int f = 0; f < F/2; f++)
        std::copy(data[f], data[f] + frame_size, old_frames[f].begin()); // 120*(2 for complex data)*(2 for oversampling by 2)

    old_pos = new_pos;
    old_path = new_path;
    old_sync_val = max_sync_val;

    if (!output_frame)
        throw spu::tools::processing_aborted();
}
}
}

#endif
