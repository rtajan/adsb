#ifndef WRAPPER_MULTIRATE_HXX_
#define WRAPPER_MULTIRATE_HXX_

#include <string>
#include "Wrapper/Module/Multirate/Multirate.hpp"

namespace py = pybind11;
using namespace py::literals;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T, bool data_complex>
void
wrap_multirate_internal(py::handle scope, const std::string& name)
{
    const char* module_doc_str = R"doc(
        Multirate module.
    )doc";
    auto py_multirate_class = py::class_<aff3ct::module::Multirate<T, data_complex>, spu::module::Stateful>(scope, name.c_str(), module_doc_str);

    std::string constructor_doc_str = R"doc(
        Constructor of Multirate module for CPLX_FLAG data type.

        Parameters
        ----------
        N : int
            The size of the input/output data.
        F : int
            The number of branches to split/merge the data.
    )doc";

    if constexpr (data_complex)
        constructor_doc_str = std::regex_replace(constructor_doc_str, std::regex("CPLX_FLAG"), "complex");
    else
        constructor_doc_str = std::regex_replace(constructor_doc_str, std::regex("CPLX_FLAG"), "real");

    py_multirate_class.def(py::init<const int, const int>(), "N"_a, "F"_a, py::return_value_policy::take_ownership, constructor_doc_str.c_str());
}

}// namespace wrapper
} // namespace adsb

#endif
