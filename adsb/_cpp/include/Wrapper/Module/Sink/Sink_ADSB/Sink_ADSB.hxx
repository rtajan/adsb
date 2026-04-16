#ifndef WRAPPER_SINK_ADSB_HXX_
#define WRAPPER_SINK_ADSB_HXX_

#include <string>
#include "Wrapper/Module/Sink/Sink_ADSB/Sink_ADSB.hpp"

namespace py = pybind11;
using namespace py::literals;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename B>
void
wrap_sink_adsb_internal(py::handle scope, const std::string& name)
{
    const char* module_doc_str = R"doc(
        Sink ADSB module.
    )doc";
    auto py_sink_adsb_class = py::class_<aff3ct::module::Sink_ADSB<B>, spu::module::Sink<B>>(scope, name.c_str(), module_doc_str);

    std::string constructor_doc_str = R"doc(
        Constructor of Sink ADSB module.
    )doc";

    py_sink_adsb_class.def(py::init<const std::string&, const std::string&, const int, const int>(), "user_name"_a, "server_addr"_a, "server_port"_a, "max_queue_size"_a = 1000, py::return_value_policy::take_ownership, constructor_doc_str.c_str());
    py_sink_adsb_class.def("flush", &aff3ct::module::Sink_ADSB<B>::flush, py::return_value_policy::automatic, "Flush the sink.");
    py_sink_adsb_class.def("print_queue", &aff3ct::module::Sink_ADSB<B>::print_queue, py::return_value_policy::automatic, "Print the message queue.");
}


}// namespace wrapper
} // namespace adsb

#endif
