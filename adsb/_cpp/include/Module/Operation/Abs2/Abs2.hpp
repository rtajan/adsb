/*!
 * \file
 * \brief Take the absolute value squared of the complex input samples.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef ABS2_HPP
#define ABS2_HPP

#include "Module/Operation/Operation.hpp"
#include <streampu.hpp>

namespace aff3ct
{
namespace module
{

/*!
 * \class Abs2
 *
 * \brief Take the absolute value squared of the complex input samples.
 *
 * \tparam R: type of the data to send or receive.
 *
 */
template <typename R = float>
class Abs2 : public Operation<R>
{
public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N: Number of complex samples.
	 */
	Abs2(const int N);
	/*!
	 * \brief Destructor.
	 */
	virtual ~Abs2() = default;

protected:
	void _perform(const R *in, R* out, const int frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Operation/Abs2/Abs2.hxx"
#endif
#endif /* ABS2_HPP */
