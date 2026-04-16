#include "Wrapper/Module/Sink/Sink_ADSB/Sink_ADSB.hpp"
#include "Tools/Type/type_list.hpp"

namespace py = pybind11;
using namespace adsb::wrapper;

void
adsb::wrapper::wrap_sink_adsb(py::handle scope)
{
    using int_lT = tl::type_list<ALL_INT_DTYPES>;
    int_lT::for_each(
      [&](auto t)
      {
          using T = typename decltype(t)::type;
          std::string T_str = spu::runtime::type_to_string[typeid(T)];
          adsb::wrapper::wrap_sink_adsb_internal<T>(scope, "Sink_ADSB_" + T_str);
      });
}
