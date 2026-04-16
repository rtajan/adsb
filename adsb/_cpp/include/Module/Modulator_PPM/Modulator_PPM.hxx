/*!
 * \file
 * \brief Modules that modulates/demodulates synchronized PPM signals.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MODULATOR_PPM_HXX_
#define MODULATOR_PPM_HXX_

#include <sstream>
#include <complex>
#include <csignal>

#include "Module/Modulator_PPM/Modulator_PPM.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
Modulator_PPM<B, R>
::Modulator_PPM(const int N, const int F)
: Stateful(),
  N(N),
  P(2*N*F), // 2 for complex data
  F(F)
{
	const std::string name = "Modulator_PPM";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (F <= 0)
	{
		std::stringstream message;
		message << "'F' has to be greater than 0 ('F' = " << F << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

    if(F%2 != 0)
    {
        std::stringstream message;
        message << "'F' has to be a multiple of 2 ('F' = " << F << ").";
        throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

	auto &p1 = this->create_task("modulate");
	auto p1s_in = this->template create_socket_in <B>(p1, "bits", N);
	auto p1s_out = this->template create_socket_out<R>(p1, "signal", P);
	this->create_codelet(p1, [p1s_in, p1s_out](spu::module::Module &m, spu::runtime::Task &t, const size_t frame_id) -> int
	{
		static_cast<Modulator_PPM<B, R>&>(m)._modulate(static_cast<B*>(t[p1s_in].get_dataptr()), static_cast<R*>(t[p1s_out].get_dataptr()), frame_id);
		return spu::runtime::status_t::SUCCESS;
	});

    auto &p2 = this->create_task("demodulate");
    auto p2s_in = this->template create_socket_in <R>(p2, "signal", P);
    auto p2s_out = this->template create_socket_out<B>(p2, "bits", N);
    this->create_codelet(p2, [p2s_in, p2s_out](spu::module::Module &m, spu::runtime::Task &t, const size_t frame_id) -> int
    {
        static_cast<Modulator_PPM<B, R>&>(m)._demodulate(static_cast<R*>(t[p2s_in].get_dataptr()), static_cast<B*>(t[p2s_out].get_dataptr()), frame_id);
        return spu::runtime::status_t::SUCCESS;
    });

}

template<typename B, typename R>
void
Modulator_PPM<B, R>::_modulate(const B* bits, R* signal, const int frame_id)
{
    for (int i = 0; i < N; i++)
    {
        std::fill(signal + i*F, signal + i*F + F, (R)bits[i]); // 2 for complex data
        std::fill(signal + i*F + F, signal + i*F + 2*F, (R)!bits[i]); // 2 for complex data
    }
}

template<typename B, typename R>
void
Modulator_PPM<B, R>::_demodulate(const R* signal, B* bits, const int frame_id)
{
    for (int i = 0; i < N; i++)
    {
        R sum0 = 0;
        R sum1 = 0;

        for (int j = 0; j < F/2; j++)
        {
            auto index_early = i*F + j; 
            auto index_late  = i*F + F/2 + j;
            sum1 += signal[2*index_early] * signal[2*index_early] + signal[2*index_early + 1] * signal[2*index_early + 1];       // 2 for complex data
            sum0 += signal[2*index_late ] * signal[2*index_late ] + signal[2*index_late  + 1] * signal[2*index_late  + 1];       // 2 for complex data
        }
        bits[i] = sum0 > sum1 ? 0 : 1;
    }
}
}
}

#endif
