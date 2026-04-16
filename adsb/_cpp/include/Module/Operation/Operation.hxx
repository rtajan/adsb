/*!
 * \file
 * \brief Sens or receive samples to or from a Radio
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef OPERATOR_HXX_
#define OPERATOR_HXX_

#include <sstream>
#include <complex>
#include <csignal>

#include "Module/Operation/Operation.hpp"

namespace aff3ct
{
namespace module
{

template <typename R>
Operation<R>
::Operation(const int N, const int M)
: Stateful(),
  N(N),
  M(M)
{
	const std::string name = "Operation";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (M <= 0)
	{
		std::stringstream message;
		message << "'M' has to be greater than 0 ('M' = " << M << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("perform");
	auto p1s_in = this->template create_socket_in <R>(p1, "in", N);
	auto p1s_out = this->template create_socket_out<R>(p1, "out", M);
	this->create_codelet(p1, [p1s_in, p1s_out](spu::module::Module &m, spu::runtime::Task &t, const size_t frame_id) -> int
	{
		static_cast<Operation<R>&>(m)._perform(static_cast<R*>(t[p1s_in].get_dataptr()), static_cast<R*>(t[p1s_out].get_dataptr()), frame_id);
		return spu::runtime::status_t::SUCCESS;
	});

}

template<typename R>
void
Operation<R>::_perform(const R* /*in_data*/, R* /*out_data*/, const int /*frame_id*/)
{
    throw spu::tools::unimplemented_error(__FILE__, __LINE__, __func__);
}
}
}


#endif
