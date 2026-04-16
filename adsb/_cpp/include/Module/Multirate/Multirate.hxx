/*!
 * \file
 * \brief Sens or receive samples to or from a Radio
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MULTIRATE_HXX_
#define MULTIRATE_HXX_

#include <sstream>
#include <complex>
#include <csignal>

#include "Module/Multirate/Multirate.hpp"

namespace aff3ct
{
namespace module
{

template <typename R, bool data_complex>
Multirate<R, data_complex>::Multirate(const int N, const int F)
: Stateful(),
  F(F)
{
	const std::string name = "Multirate";
	this->set_name(name);
	this->set_short_name(name);


    auto R_N = data_complex ? 2 * N : N;

    if (R_N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << R_N << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (F <= 0)
	{
		std::stringstream message;
		message << "'F' has to be greater than 0 ('F' = " << F << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

    if (R_N % F != 0)
    {
        std::stringstream message;
        message << "'N' has to be a multiple of 'F' ('N' = " << R_N << ", 'F' = " << F << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

    auto S_N = N / F; // split output size
    auto R_S_N = data_complex ? 2 * S_N : S_N;

	auto &p1 = this->create_task("split");
	auto p1s_in = this->template create_socket_in <R>(p1, "in", R_N);

	std::vector<size_t> p1s_out(F,0);
    for (int f = 0; f < F; f++)
        p1s_out[f] = this->template create_socket_out<R>(p1, "out" + std::to_string(f), R_S_N);

    this->create_codelet(p1, [S_N, F, p1s_in, p1s_out](spu::module::Module &m, spu::runtime::Task &t, const size_t frame_id) -> int
    {
    if constexpr (data_complex)
    {

            for (int f = 0; f < F; f++)
            {
                auto f_out = static_cast<R*>(t[p1s_out[f]].get_dataptr());
                auto in    = static_cast<R*>(t[p1s_in].get_dataptr());

                for (int s = 0; s < S_N; s++)
                {
                    f_out[2 * s    ] = in[2 * F * s + 2 * f    ]; // Real part
                    f_out[2 * s + 1] = in[2 * F * s + 2 * f + 1]; // Imaginary part

                }
            }
    }
    else
    {
            for (int f = 0; f < F; f++)
            {
                auto f_out = static_cast<R*>(t[p1s_out[f]].get_dataptr());
                auto in    = static_cast<R*>(t[p1s_in].get_dataptr());

                for (int s = 0; s < S_N; s++)
                    f_out[s] = in[F * s + f];
            }
    }
    return spu::runtime::status_t::SUCCESS;
    });



    auto &p2 = this->create_task("merge");
    auto p2s_out = this->template create_socket_out<R>(p2, "out", R_N);
    std::vector<size_t> p2s_in(F,0);
    for (int f = 0; f < F; f++)
        p2s_in[f] = this->template create_socket_in <R>(p2, "in" + std::to_string(f), R_S_N);

    this->create_codelet(p2, [S_N, F, p2s_in, p2s_out](spu::module::Module &m, spu::runtime::Task &t, const size_t frame_id) -> int
    {
    if constexpr (data_complex)
    {

            auto out = static_cast<R*>(t[p2s_out].get_dataptr());

            for (int f = 0; f < F; f++)
            {
                auto f_in = static_cast<R*>(t[p2s_in[f]].get_dataptr());

                for (int s = 0; s < S_N; s++)
                {
                    out[2 * F * s + 2*f    ] = f_in[2 * s    ]; // Real part
                    out[2 * F * s + 2*f + 1] = f_in[2 * s + 1]; // Imaginary part
                }
            }
    }
    else
    {
            auto out = static_cast<R*>(t[p2s_out].get_dataptr());

            for (int f = 0; f < F; f++)
            {
                auto f_in = static_cast<R*>(t[p2s_in[f]].get_dataptr());
                for (int s = 0; s < S_N; s++)
                    out[F * s + f] = f_in[s];
            }
    }
    return spu::runtime::status_t::SUCCESS;
    });

}

}
}


#endif
