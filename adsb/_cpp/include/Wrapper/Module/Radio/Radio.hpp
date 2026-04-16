#ifndef WRAPPER_RADIO_HPP_
#define WRAPPER_RADIO_HPP_

#include <streampu.hpp>
#include <pybind11/pybind11.h>

#include "Module/Radio/Radio.hpp"

namespace py = pybind11;
using namespace spu;
using namespace spu::module;

namespace adsb
{
namespace wrapper
{

template<typename T = int>
class Trampoline_Radio : aff3ct::module::Radio<T>//, public py::trampoline_self_life_support
{
  public:
    using aff3ct::module::Radio<T>::Radio;
    void _send(const T *X_N1, const int frame_id) override
    {
        PYBIND11_OVERRIDE_PURE(void,                  /* Return type */
                               aff3ct::module::Radio<T>, /* Parent class */
                               _send,                 /* Name of function in C++ (must
                                                         match Python name) */
                               X_N1, frame_id        /* Argument(s) (...) */
        );
    }

    void _receive(T *Y_N1, const int frame_id) override
    {
        PYBIND11_OVERRIDE_PURE(void,                  /* Return type */
                               aff3ct::module::Radio<T>, /* Parent class */
                               _receive,                 /* Name of function in C++ (must
                                                         match Python name) */
                               Y_N1, frame_id        /* Argument(s) (...) */
        );
    }
};

template<typename T = int>
class Publicist_Radio : aff3ct::module::Radio<T>
{
  public:
    using aff3ct::module::Radio<T>::Radio;
    using aff3ct::module::Radio<T>::_send;
    using aff3ct::module::Radio<T>::_receive;
};


template<typename T = int>
inline void
wrap_radio_internal(py::handle scope, const std::string& name);

void
wrap_radio(py::handle scope);

} // namespace wrapper
} // namespace adsb



#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Wrapper/Module/Radio/Radio.hxx"
#endif
#endif
