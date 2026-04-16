#ifndef WRAPPER_FRAME_SYNCHRONIZER_ADSB_HXX_
#define WRAPPER_FRAME_SYNCHRONIZER_ADSB_HXX_

#include <string>
#include "Wrapper/Module/Frame_Synchronizer_ADSB/Frame_Synchronizer_ADSB.hpp"

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
wrap_frame_synchronizer_adsb_internal(py::handle scope, const std::string& name)
{
    const char* module_doc_str = R"doc(
        Frame Synchronizer ADSB module.
    )doc";
    auto py_frame_synchronizer_adsb_class = py::class_<aff3ct::module::Frame_Synchronizer_ADSB<T>, spu::module::Stateful>(scope, name.c_str(), module_doc_str);

    std::string constructor_doc_str = R"doc(
        Constructor of Frame Synchronizer ADSB module.

        Parameters
        ----------
        F : int
            The number of branches for the data.
        threshold : real
            The threshold value for the frame synchronization.
    )doc";

    py_frame_synchronizer_adsb_class.def(py::init<const int, const float>(), "F"_a, "threshold"_a, py::return_value_policy::take_ownership, constructor_doc_str.c_str());
}

}// namespace wrapper
} // namespace adsb

#endif
