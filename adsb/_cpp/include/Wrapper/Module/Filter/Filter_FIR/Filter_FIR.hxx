#ifndef WRAPPER_FILTER_FIR_HXX_
#define WRAPPER_FILTER_FIR_HXX_

#include "Wrapper/Module/Filter/Filter.hpp"
#include "Wrapper/Module/Filter/Filter_FIR/Filter_FIR.hpp"
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
wrap_filter_fir_internal(py::handle scope, const std::string& name)
{
    const char* class_doc_str = R"doc(
        FIR Filter class.

        Task `filter` applies a Finite Impulse Response (FIR) filter to a signal.
        The impulse response is defined by the coefficients provided during the instantiation of the class.

    )doc";
    auto py_filter_class = py::class_<aff3ct::module::Filter_FIR<T, complex_data>,
                                      aff3ct::module::Filter<T>>(scope, name.c_str(), class_doc_str);

    auto constructor_doc_str = R"doc(
        Constructor.

        Parameters
        ----------
        N : int
            Number of samples at filter's input/output.

        h : list of filter coefficients (reals).

    )doc";
    py_filter_class.def(py::init<const size_t, const std::vector<T>&>(), "N"_a, "h"_a, constructor_doc_str, py::return_value_policy::take_ownership);
}


}// namespace wrapper
} // namespace adsb

#endif

