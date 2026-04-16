#ifndef WRAPPER_FILTER_HXX_
#define WRAPPER_FILTER_HXX_

#include "Wrapper/Module/Filter/Filter.hpp"

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
wrap_filter_internal(py::handle scope, const std::string& name)
{
    const char* doc_str = R"doc(
        Abstract Filter class.
    )doc";
    auto py_filter_class = py::class_<aff3ct::module::Filter<T>, spu::module::Stateful>(scope, name.c_str(), doc_str);
    //py_filter_class.def(py::init<const size_t, const size_t>(), "N"_a, "N_fil"_a, py::return_value_policy::take_ownership);
}

}// namespace wrapper
} // namespace adsb

#endif
