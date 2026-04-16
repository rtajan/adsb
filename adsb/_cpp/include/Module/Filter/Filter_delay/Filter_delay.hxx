/*!
 * \file
 * \brief Filters a signal.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef FILTER_DELAY_HXX_
#define FILTER_DELAY_HXX_

#include <string>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <sstream>

#include "Module/Filter/Filter_delay/Filter_delay.hpp"

namespace aff3ct
{
namespace module
{

template <typename R, bool data_complex>
Filter_delay<R, data_complex>::
Filter_delay(const int N, const int delay)
: aff3ct::module::Filter<R>((data_complex ? 2 : 1) * N, (data_complex ? 2 : 1) * N), delay(delay), buffer((data_complex ? 2 : 1) * delay, (R)0)
{
	const std::string name = "Filter_delay";
	this->set_name(name);
	this->set_short_name(name);


	if (N < delay)
	{
		std::stringstream message;
		message << "'N' has to be greater than 'delay' ('N' = " << N << ", 'delay' = " << delay << ").";
		throw spu::tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename R, bool data_complex>
void Filter_delay<R, data_complex>::reset()
{
	std::fill(buffer.begin(), buffer.end(), (R)0);
}

template <typename R, bool data_complex>
void Filter_delay<R, data_complex>::_filter(const R *X_N1, R *Y_N2, const int frame_id)
{
	if constexpr (data_complex)
	{
		// Copy the delayed samples from the buffer to the output
		std::copy(buffer.begin(), buffer.end(), Y_N2);

		// Copy the new samples from the input to the output, after the delayed samples
		std::copy(X_N1, X_N1 + (this->N - 2 * delay), Y_N2 + 2 * delay);

		// Update the buffer with the last 'delay' samples from the input
		std::copy(X_N1 + (this->N - 2 * delay), X_N1 + this->N, buffer.begin());
	}
	else
	{
		// Copy the delayed samples from the buffer to the output
		std::copy(buffer.begin(), buffer.end(), Y_N2);

		// Copy the new samples from the input to the output, after the delayed samples
		std::copy(X_N1, X_N1 + (this->N - delay), Y_N2 + delay);

		// Update the buffer with the last 'delay' samples from the input
		std::copy(X_N1 + (this->N - delay), X_N1 + this->N, buffer.begin());
	}
}


}
}

#endif
