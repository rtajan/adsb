/*!
 * \file
 * \brief Multirate module, i.e. a module that can separate/combine data to multiple rates.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MULTIRATE_HPP
#define MULTIRATE_HPP

#include <streampu.hpp>

namespace aff3ct
{
namespace module
{


/*!
 * \class Multirate
 *
 * \brief Multirate module, i.e. a module that can separate/combine data to multiple rates.
 *
 * \tparam R: data type.
 *
 */
template <typename R = float, bool data_complex = false>
class Multirate : public spu::module::Stateful
{
protected:
	const int F; /*!< rate factor */

public:
	/*!
	 * \brief Constructor.
	 *
     * \param N: The number of input elements.
	 * \param F: Rate factor.
	 */
	Multirate(const int N, const int F);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Multirate() = default;

};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Multirate/Multirate.hxx"
#endif
#endif /* MULTI_HPP */
