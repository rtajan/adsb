
#ifndef WRAPPER_FILTER_FIR_HPP_
#define WRAPPER_FILTER_FIR_HPP_

#include <streampu.hpp>
#include <pybind11/pybind11.h>

#include "Module/Filter/Filter_FIR/Filter_FIR.hpp"

namespace py = pybind11;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T = int, bool complex_data = false>
inline void
wrap_filter_fir_internal(py::handle scope, const std::string& name);

void
wrap_filter_fir(py::handle scope);

} // namespace wrapper
} // namespace adsb



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Filter/Filter_FIR/Filter_FIR.hxx"
#endif
#endif
