#ifndef WRAPPER_OPERATION_HXX_
#define WRAPPER_OPERATION_HXX_

#include "Wrapper/Module/Operation/Operation.hpp"

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
wrap_operation_internal(py::handle scope, const std::string& name)
{
    const char* doc_str = R"doc(
        Abstract Operation class.
    )doc";
    auto py_operation_class = py::class_<aff3ct::module::Operation<T>, spu::module::Stateful>(scope, name.c_str(), doc_str);
}

}// namespace wrapper
} // namespace adsb

#endif
