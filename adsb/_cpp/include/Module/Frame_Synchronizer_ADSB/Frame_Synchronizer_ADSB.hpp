/*!
 * \file
 * \brief Frame synchronizer module for ADSB, 
 * i.e. a module that can recover an ADSB frame from F streams of data.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef FRAME_SYNCHRONIZER_ADSB_HPP
#define FRAME_SYNCHRONIZER_ADSB_HPP

#include <streampu.hpp>

namespace aff3ct
{
namespace module
{


/*!
 * \class Frame_Synchronizer_ADSB
 *
 * \brief Frame synchronizer module for ADSB, 
 * i.e. a module that can recover an ADSB frame from F streams of data.
 *
 * \tparam R: data type.
 *
 */

template <typename R = float>
class Frame_Synchronizer_ADSB : public spu::module::Stateful
{
protected:
	const int F; /*!< Number of paths */
	const R threshold; /*!< Threshold value */

	std::vector<std::vector<R>> old_frames;
	int32_t old_pos;
	int32_t old_path;
	R old_sync_val;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param F: Number of paths.
	 * \param threshold: Threshold value.
	 */
	Frame_Synchronizer_ADSB(const int F, const R threshold);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Frame_Synchronizer_ADSB() = default;

protected:
	void _synchronize(std::vector<const R*> data, std::vector<const R*> num, std::vector<const R*> denom, R* out, R* sync_val, int32_t* pos, int32_t* path, const int frame_id);

};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Frame_Synchronizer_ADSB/Frame_Synchronizer_ADSB.hxx"
#endif
#endif /* MULTI_HPP */
