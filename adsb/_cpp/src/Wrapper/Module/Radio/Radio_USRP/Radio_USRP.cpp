#include "Wrapper/Module/Radio/Radio_USRP/Radio_USRP.hpp"

#include "Tools/Type/type_list.hpp"

namespace py = pybind11;
using namespace adsb::wrapper;

void
adsb::wrapper::wrap_radio_usrp(py::handle scope)
{


    py::class_<aff3ct::module::USRP_params>(scope, "USRP_params")
        .def(py::init<>())
        .def_readwrite("N", &aff3ct::module::USRP_params::N)
        .def_readwrite("threaded", &aff3ct::module::USRP_params::threaded)
        .def_readwrite("fifo_size", &aff3ct::module::USRP_params::fifo_size)
        .def_readwrite("type", &aff3ct::module::USRP_params::type)
        .def_readwrite("usrp_addr", &aff3ct::module::USRP_params::usrp_addr)
        .def_readwrite("clk_rate", &aff3ct::module::USRP_params::clk_rate)
        .def_readwrite("rx_enabled", &aff3ct::module::USRP_params::rx_enabled)
        .def_readwrite("rx_rate", &aff3ct::module::USRP_params::rx_rate)
        .def_readwrite("rx_subdev_spec", &aff3ct::module::USRP_params::rx_subdev_spec)
        .def_readwrite("rx_antenna", &aff3ct::module::USRP_params::rx_antenna)
        .def_readwrite("rx_freq", &aff3ct::module::USRP_params::rx_freq)
        .def_readwrite("rx_gain", &aff3ct::module::USRP_params::rx_gain)
        .def_readwrite("rx_filepath", &aff3ct::module::USRP_params::rx_filepath)
        .def_readwrite("tx_filepath", &aff3ct::module::USRP_params::tx_filepath)
        .def_readwrite("tx_enabled", &aff3ct::module::USRP_params::tx_enabled)
        .def_readwrite("tx_rate", &aff3ct::module::USRP_params::tx_rate)
        .def_readwrite("tx_subdev_spec", &aff3ct::module::USRP_params::tx_subdev_spec)
        .def_readwrite("tx_antenna", &aff3ct::module::USRP_params::tx_antenna)
        .def_readwrite("tx_freq", &aff3ct::module::USRP_params::tx_freq)
        .def_readwrite("tx_gain", &aff3ct::module::USRP_params::tx_gain);

    using int_lT = tl::type_list<double, float, int16_t, int8_t>;
    int_lT::for_each(
      [&](auto t)
      {
          using T = typename decltype(t)::type;
          std::string T_str = spu::runtime::type_to_string[typeid(T)];
          adsb::wrapper::wrap_radio_usrp_internal<T>(scope, "Radio_USRP_" + T_str);
        
      });


}
