
#ifndef WRAPPER_OPERATION_HPP_
#define WRAPPER_OPERATION_HPP_

#include <streampu.hpp>
#include <pybind11/pybind11.h>

#include "Module/Operation/Operation.hpp"

namespace py = pybind11;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T = int>
inline void
wrap_operation_internal(py::handle scope, const std::string& name);

void
wrap_operation(py::handle scope);

} // namespace wrapper
} // namespace adsb



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Operation/Operation.hxx"
#endif
#endif
