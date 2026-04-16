#ifndef WRAPPER_ABS2_HXX_
#define WRAPPER_ABS2_HXX_

#include "Wrapper/Module/Operation/Abs2/Abs2.hpp"
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
wrap_abs2_internal(py::handle scope, const std::string& name)
{
    const char* class_doc_str = R"doc(
        Absolute Value class.

        Task `abs2` computes the squared absolute value of a complex signal.
    )doc";
    auto py_abs2_class = py::class_<aff3ct::module::Abs2<T>, 
                                     aff3ct::module::Operation<T>>(scope, name.c_str(), class_doc_str);

    auto constructor_doc_str = R"doc(
        Constructor.

        Parameters
        ----------
        N : int
            Number of complex samples at input.
    )doc";
    py_abs2_class.def(py::init<const size_t>(), "N"_a, constructor_doc_str, py::return_value_policy::take_ownership);
}

}// namespace wrapper
} // namespace adsb

#endif
