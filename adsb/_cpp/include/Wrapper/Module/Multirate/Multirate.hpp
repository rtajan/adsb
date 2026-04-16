
#ifndef WRAPPER_MULTIRATE_HPP_
#define WRAPPER_MULTIRATE_HPP_

#include <streampu.hpp>
#include <pybind11/pybind11.h>

#include "Module/Multirate/Multirate.hpp"

namespace py = pybind11;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T = int, bool data_complex = false>
inline void
wrap_multirate_internal(py::handle scope, const std::string& name);

void
wrap_multirate(py::handle scope);

} // namespace wrapper
} // namespace adsb



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Multirate/Multirate.hxx"
#endif
#endif
