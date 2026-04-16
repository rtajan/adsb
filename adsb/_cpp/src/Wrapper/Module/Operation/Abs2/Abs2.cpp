#include "Wrapper/Module/Operation/Abs2/Abs2.hpp"

#include "Tools/Type/type_list.hpp"

namespace py = pybind11;
using namespace adsb::wrapper;

void
adsb::wrapper::wrap_abs2(py::handle scope)
{
    using int_lT = tl::type_list<ALL_DTYPES>;
    int_lT::for_each(
      [&](auto t)
      {
          using T = typename decltype(t)::type;
          std::string T_str = spu::runtime::type_to_string[typeid(T)];
          adsb::wrapper::wrap_abs2_internal<T>(scope, "Abs2_" + T_str);
      });
}
