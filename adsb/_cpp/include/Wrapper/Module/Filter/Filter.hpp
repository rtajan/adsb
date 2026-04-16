
#ifndef WRAPPER_FILTER_HPP_
#define WRAPPER_FILTER_HPP_

#include <streampu.hpp>
#include <pybind11/pybind11.h>

#include "Module/Filter/Filter.hpp"

namespace py = pybind11;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T = int>
inline void
wrap_filter_internal(py::handle scope, const std::string& name);

void
wrap_filter(py::handle scope);

} // namespace wrapper
} // namespace adsb



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Filter/Filter.hxx"
#endif
#endif
