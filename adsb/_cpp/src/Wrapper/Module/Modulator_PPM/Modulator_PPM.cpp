#include "Wrapper/Module/Modulator_PPM/Modulator_PPM.hpp"
#include "Tools/Type/type_list.hpp"

namespace py = pybind11;
using namespace adsb::wrapper;

void
adsb::wrapper::wrap_modulator_ppm(py::handle scope)
{
    using int_lB = tl::type_list<ALL_INT_DTYPES>;
    using int_lR = tl::type_list<FLOAT_DTYPES>;
    int_lB::for_each(
      [&](auto b)
      {

        using B = typename decltype(b)::type;
        std::string B_str = spu::runtime::type_to_string[typeid(B)];
        int_lR::for_each(
            [&](auto r)
            {
                using R = typename decltype(r)::type;
                std::string R_str = spu::runtime::type_to_string[typeid(R)];
                adsb::wrapper::wrap_modulator_ppm_internal<B, R>(scope, "Modulator_PPM_" + B_str + "_" + R_str);
            });
    });
}