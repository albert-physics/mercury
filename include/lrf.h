#ifndef lrf_h
#define lrf_h

#include <pybind11/pybind11.h>
namespace py = pybind11;

void init_lrf(py::module& m);

#endif