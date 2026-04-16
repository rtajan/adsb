#include "Wrapper/Module/Filter/Filter_delay/Filter_delay.hpp"

#include "Tools/Type/type_list.hpp"

namespace py = pybind11;
using namespace adsb::wrapper;

void
adsb::wrapper::wrap_filter_delay(py::handle scope)
{
    using int_lT = tl::type_list<ALL_DTYPES>;
    int_lT::for_each(
      [&](auto t)
      {
          using T = typename decltype(t)::type;
          std::string T_str = spu::runtime::type_to_string[typeid(T)];
          adsb::wrapper::wrap_filter_delay_internal<T, true>(scope, "Filter_delay_" + T_str + "_C");
          adsb::wrapper::wrap_filter_delay_internal<T, false>(scope, "Filter_delay_" + T_str + "_R");
      });


}
