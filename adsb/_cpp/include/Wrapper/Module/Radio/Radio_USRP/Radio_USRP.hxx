#ifndef WRAPPER_RADIO_USRP_HXX_
#define WRAPPER_RADIO_USRP_HXX_

#include "Wrapper/Module/Radio/Radio_USRP/Radio_USRP.hpp"
#include "Module/Radio/Radio.hpp"
#include "Module/Radio/Radio_USRP/Radio_USRP.hpp"


#include <streampu.hpp>

namespace py = pybind11;
using namespace py::literals;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T>
void
wrap_radio_usrp_internal(py::handle scope, const std::string& name)
{
    const char* doc_str = R"doc(
        Radio_USRP module.
    )doc";
    auto py_radio_usrp_class = py::class_<aff3ct::module::Radio_USRP<T>, aff3ct::module::Radio<T>, spu::tools::Interface_waiting, spu::tools::Interface_reset>(scope, name.c_str(), doc_str);
    py_radio_usrp_class.def(py::init<const aff3ct::module::USRP_params&>(), "params"_a, py::return_value_policy::take_ownership);
}

}// namespace wrapper
} // namespace adsb

#endif
