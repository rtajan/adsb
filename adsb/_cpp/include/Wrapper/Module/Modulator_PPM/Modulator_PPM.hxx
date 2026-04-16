#ifndef WRAPPER_MODULATOR_PPM_HXX_
#define WRAPPER_MODULATOR_PPM_HXX_

#include <string>
#include "Wrapper/Module/Modulator_PPM/Modulator_PPM.hpp"

namespace py = pybind11;
using namespace py::literals;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename B, typename R>
void
wrap_modulator_ppm_internal(py::handle scope, const std::string& name)
{
    const char* module_doc_str = R"doc(
        Modulator PPM module.
    )doc";
    auto py_modulator_ppm_class = py::class_<aff3ct::module::Modulator_PPM<B,R>, spu::module::Stateful>(scope, name.c_str(), module_doc_str);

    std::string constructor_doc_str = R"doc(
        Constructor of Modulator PPM module.
    )doc";

    py_modulator_ppm_class.def(py::init<const int, const int>(), "N"_a, "F"_a, py::return_value_policy::take_ownership, constructor_doc_str.c_str());
}

}// namespace wrapper
} // namespace adsb

#endif
