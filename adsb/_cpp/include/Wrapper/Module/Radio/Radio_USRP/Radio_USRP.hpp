#ifndef WRAPPER_RADIO_USRP_HPP_
#define WRAPPER_RADIO_USRP_HPP_

#include "Module/Radio/Radio_USRP/Radio_USRP.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;


namespace adsb
{
namespace wrapper
{

template<typename T = int>
inline void
wrap_radio_usrp_internal(py::handle scope, const std::string& name);

void
wrap_radio_usrp(py::handle scope);

} // namespace wrapper
} // namespace adsb

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Radio/Radio_USRP/Radio_USRP.hxx"
#endif

#endif  // WRAPPER_RADIO_USRP_HPP_