
#ifndef WRAPPER_ABS2_HPP_
#define WRAPPER_ABS2_HPP_

#include <streampu.hpp>
#include <pybind11/pybind11.h>

#include "Module/Operation/Abs2/Abs2.hpp"

namespace py = pybind11;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T = int, bool complex_data = false>
inline void
wrap_abs2_internal(py::handle scope, const std::string& name);

void
wrap_abs2(py::handle scope);

} // namespace wrapper
} // namespace adsb



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Operation/Abs2/Abs2.hxx"
#endif
#endif
