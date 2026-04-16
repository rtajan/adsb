#ifndef WRAPPER_SINK_ADSB_HPP_
#define WRAPPER_SINK_ADSB_HPP_

#include <streampu.hpp>
#include <pybind11/pybind11.h>

#include "Module/Sink/Sink_ADSB/Sink_ADSB.hpp"

namespace py = pybind11;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename B = int>
inline void
wrap_sink_adsb_internal(py::handle scope, const std::string& name);

void
wrap_sink_adsb(py::handle scope);

} // namespace wrapper
} // namespace adsb



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Sink/Sink_ADSB/Sink_ADSB.hxx"
#endif
#endif
