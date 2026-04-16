#ifndef WRAPPER_FILTER_DELAY_HXX_
#define WRAPPER_FILTER_DELAY_HXX_

#include "Wrapper/Module/Filter/Filter.hpp"
#include "Wrapper/Module/Filter/Filter_delay/Filter_delay.hpp"
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace py::literals;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T, bool complex_data>
void
wrap_filter_delay_internal(py::handle scope, const std::string& name)
{
    const char* class_doc_str = R"doc(
        Delay Filter class.

        Task `filter` delays a signal by a predefined number of samples.
    )doc";
    auto py_filter_class = py::class_<aff3ct::module::Filter_delay<T, complex_data>,
                                      aff3ct::module::Filter<T>>(scope, name.c_str(), class_doc_str);

    auto constructor_doc_str = R"doc(
        Constructor.

        Parameters
        ----------
        N : int
            Number of samples at filter's input/output.

        delay : int
            Delay introduced by the filter (in number of samples).

    )doc";
    py_filter_class.def(py::init<const size_t, const int>(), "N"_a, "delay"_a, constructor_doc_str, py::return_value_policy::take_ownership);
}


}// namespace wrapper
} // namespace adsb

#endif

