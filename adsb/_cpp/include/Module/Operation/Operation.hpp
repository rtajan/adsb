/*!
 * \file
 * \brief Transmit or receive data to or from a radio module.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <streampu.hpp>

namespace aff3ct
{
namespace module
{


/*!
 * \class Operation
 *
 * \brief Transmit or receive data to or from a radio module.
 *
 * \tparam R: type of the data to send or receive.
 *
 */
template <typename R = float>
class Operation : public spu::module::Stateful
{
protected:
	const int N; /*!< Size of one frame (= number of samples in one frame) */
	const int M; /*!< Size of one frame (= number of samples in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:     Operation frame length.
	 * \param M:     Operation frame length.
	 */
	Operation(const int N, const int M);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Operation() = default;

protected:
	virtual void _perform(const R* in, R* out, const int frame_id) = 0;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Operation/Operation.hxx"
#endif
#endif /* OPERATION_HPP */
