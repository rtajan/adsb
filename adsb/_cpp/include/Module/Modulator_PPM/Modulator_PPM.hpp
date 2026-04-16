/*!
 * \file
 * \brief Modules that modulates/demodulates synchronized PPM signals.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MODULATOR_PPM_HPP
#define MODULATOR_PPM_HPP

#include <streampu.hpp>

namespace aff3ct
{
namespace module
{


/*!
 * \class Operation
 *
 * \brief Modules that modulates/demodulates synchronized ADSB signals.
 *
 * \tparam R: type of the data
 *
 */
template <typename B = int8_t, typename R = float>
class Modulator_PPM : public spu::module::Stateful
{
protected:
	const int N; /*!< Size of one frame in bits */
	const int P; /*!< Size of one frame in complex samples */
	const int F; /*!< Oversampling factor (number of samples per bit) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:     Operation frame length.
	 * \param P:     Operation frame length.
	 * \param F:     Oversampling factor.
	 */
	Modulator_PPM(const int N, const int F);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Modulator_PPM() = default;

protected:
	void _modulate(const B* in, R* out, const int frame_id);
	void _demodulate(const R* in, B* out, const int frame_id);

};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Modulator_PPM/Modulator_PPM.hxx"
#endif
#endif /* MODULATOR_PPM_HPP */
