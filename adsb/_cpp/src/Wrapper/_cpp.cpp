#include "Wrapper/_cpp.hpp"

#include <string>
#include <thread>

#include <streampu.hpp>

namespace py = pybind11;
using namespace spu;
using namespace py::literals;

#define PYBIND11_DETAILED_ERROR_MESSAGES

PYBIND11_MODULE(_cpp, m)
{
    py::object pyspu_stateful = (py::object)py::module_::import("streampu");
    spu::tools::Buffer_allocator::set_task_autoalloc(true);

    m.doc() = "ADSB module implemented in C++";
    adsb::wrapper::wrap_radio(m);

    adsb::wrapper::wrap_filter(m);

    adsb::wrapper::wrap_operation(m);

    adsb::wrapper::wrap_multirate(m);

    adsb::wrapper::wrap_frame_synchronizer_adsb(m);

    adsb::wrapper::wrap_modulator_ppm(m);

    adsb::wrapper::wrap_sink_adsb(m);
}