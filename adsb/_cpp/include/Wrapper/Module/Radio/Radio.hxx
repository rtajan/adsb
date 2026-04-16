#ifndef WRAPPER_RADIO_HXX_
#define WRAPPER_RADIO_HXX_

#include "Wrapper/Module/Radio/Radio.hpp"

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
wrap_radio_internal(py::handle scope, const std::string& name)
{
    const char* doc_str = R"doc(
        Radio module.

        Parameters
        ----------
        N : int
            Size of one frame (number of complex samples in one frame).

        Methods
        -------
        _send
            Send samples to the radio. This method must be implemented in a derived class.
        _receive
            Receive samples from the radio. This method must be implemented in a derived class.

        )doc";
    auto py_radio_class = py::class_<aff3ct::module::Radio<T>, spu::module::Stateful, spu::tools::Interface_is_done, adsb::wrapper::Trampoline_Radio<T>>(scope, name.c_str());
    py_radio_class.def(py::init<const size_t>(), "N"_a, doc_str, py::return_value_policy::take_ownership);
    py_radio_class.def("_send", &adsb::wrapper::Publicist_Radio<T>::_send, "X_N1"_a, "frame_id"_a = 0);
    py_radio_class.def("_receive", &adsb::wrapper::Publicist_Radio<T>::_receive, "Y_N1"_a, "frame_id"_a = 0);
    py_radio_class.def_property("done_flag", &aff3ct::module::Radio<T>::get_done_flag, &aff3ct::module::Radio<T>::set_done_flag);

}

}// namespace wrapper
} // namespace adsb

#endif
