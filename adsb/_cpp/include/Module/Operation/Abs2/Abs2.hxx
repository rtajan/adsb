/*!
 * \file
 * \brief Sens or receive samples to or from a Radio
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef ABS2_HXX_
#define ABS2_HXX_

#include <sstream>
#include <complex>
#include <csignal>

#include "Module/Operation/Abs2/Abs2.hpp"
#include "mipp.h"

namespace aff3ct
{
namespace module
{

template <typename R>
Abs2<R>
::Abs2(const int N)
: Operation<R>(N*2, N)
{
	const std::string name = "Abs2";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

    if (N % 2 != 0)
	{
		std::stringstream message;
		message << "'N' has to be even ('N' = " << N << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

}

template<typename R>
void
Abs2<R>::_perform(const R* in, R* out, const int frame_id)
{
    for (int i = 0; i < this->M; i++) //= mipp::N<R>()
	{
		out[i] = in[2*i]*in[2*i] + in[2*i+1]*in[2*i+1];
	}
    //{
    //    mipp::Regx2<R> in_reg;
	//	in_reg.load(in + i + i);
    //    mipp::Reg<R> out_reg = mipp::norm(in_reg);
    //    mipp::store(out + i, out_reg);
    //}
    //auto rest = this->M % mipp::N<R>();
    //for (int i = this->M - rest; i < this->M; i++)
    //    out[i] = in[2*i]*in[2*i] + in[2*i+1]*in[2*i+1];
}
}
}


#endif
