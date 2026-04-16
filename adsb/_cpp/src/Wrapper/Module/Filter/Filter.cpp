#include "Wrapper/Module/Filter/Filter.hpp"
#include "Wrapper/Module/Filter/Filter_FIR/Filter_FIR.hpp"
#include "Wrapper/Module/Filter/Filter_delay/Filter_delay.hpp"
#include "Tools/Type/type_list.hpp"

namespace py = pybind11;
using namespace adsb::wrapper;

void
adsb::wrapper::wrap_filter(py::handle scope)
{
    using int_lT = tl::type_list<ALL_DTYPES>;
    int_lT::for_each(
      [&](auto t)
      {
          using T = typename decltype(t)::type;
          std::string T_str = spu::runtime::type_to_string[typeid(T)];
          adsb::wrapper::wrap_filter_internal<T>(scope, "Filter_" + T_str);
      });

      adsb::wrapper::wrap_filter_fir(scope);
      adsb::wrapper::wrap_filter_delay(scope);
}
