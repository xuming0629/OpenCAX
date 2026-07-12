#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(opencax, m)
{
    m.doc() = "OpenCAX Python bindings";

    m.def("version", []()
    {
        return "0.1.0";
    });

    m.def("add", [](double a, double b)
    {
        return a + b;
    });
}