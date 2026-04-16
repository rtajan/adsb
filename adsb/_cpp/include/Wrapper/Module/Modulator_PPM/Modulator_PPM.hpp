#ifndef WRAPPER_MODULATOR_PPM_HPP_
#define WRAPPER_MODULATOR_PPM_HPP_

#include <streampu.hpp>
#include <pybind11/pybind11.h>

#include "Module/Modulator_PPM/Modulator_PPM.hpp"

namespace py = pybind11;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename B = int, typename R = float>
inline void
wrap_modulator_ppm_internal(py::handle scope, const std::string& name);

void
wrap_modulator_ppm(py::handle scope);

} // namespace wrapper
} // namespace adsb



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Modulator_PPM/Modulator_PPM.hxx"
#endif
#endif
