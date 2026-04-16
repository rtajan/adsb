
#ifndef WRAPPER_FRAME_SYNCHRONIZER_ADSB_HPP_
#define WRAPPER_FRAME_SYNCHRONIZER_ADSB_HPP_

#include <streampu.hpp>
#include <pybind11/pybind11.h>

#include "Module/Frame_Synchronizer_ADSB/Frame_Synchronizer_ADSB.hpp"

namespace py = pybind11;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T = int>
inline void
wrap_frame_synchronizer_adsb_internal(py::handle scope, const std::string& name);

void
wrap_frame_synchronizer_adsb(py::handle scope);

} // namespace wrapper
} // namespace adsb



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Frame_Synchronizer_ADSB/Frame_Synchronizer_ADSB.hxx"
#endif
#endif
