#include "Wrapper/Module/Radio/Radio.hpp"
#include "Wrapper/Module/Radio/Radio_USRP/Radio_USRP.hpp"

#include "Tools/Type/type_list.hpp"

namespace py = pybind11;
using namespace adsb::wrapper;

void
adsb::wrapper::wrap_radio(py::handle scope)
{
    using int_lT = tl::type_list<ALL_DTYPES>;
    int_lT::for_each(
      [&](auto t)
      {
          using T = typename decltype(t)::type;
          std::string T_str = spu::runtime::type_to_string[typeid(T)];
          adsb::wrapper::wrap_radio_internal<T>(scope, "Radio_" + T_str);
      });

      adsb::wrapper::wrap_radio_usrp(scope);
}
