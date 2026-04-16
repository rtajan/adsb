/*!
 * \file
 * \brief Filters a signal.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef FILTER_DELAY_HPP_
#define FILTER_DELAY_HPP_

#include <vector>

#include <streampu.hpp>
#include "Module/Filter/Filter.hpp"

namespace aff3ct
{
namespace module
{

/*!
 * \class Filter_delay
 *
 * \brief Delay a signal by a certain number of samples.
 *
 * \tparam R: type of the reals (floating-point representation) of the filtering process.
 * \tparam data_complex: if true, the data is complex (interleaved format).
 *
 */
template <typename R = float, bool data_complex = false>
class Filter_delay : public aff3ct::module::Filter<R>
{

protected:
	const int delay; /*!< Delay introduced by the filter (in number of samples) */

	std::vector<R> buffer; /*!< Buffer to store the previous samples for the delay */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame (= number of samples in one frame).
	 * \param delay:    delay introduced by the filter (in number of samples).
	 */
	Filter_delay(const int N, const int delay);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Filter_delay() = default;

	void reset();

protected:
	virtual void _filter(const R *X_N1,  R *Y_N2, const int frame_id);
};
}
}
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Filter_delay.hxx"
#endif
#endif /* FILTER_DELAY_HPP_ */
