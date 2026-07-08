#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "lrfxyz.h"
#include "lrfxy.h"
#include "lrfaxial.h"
#include "lrf.h"


namespace py = pybind11;
using namespace pybind11::literals; 

void init_lrf(py::module& m){
    m.doc() = "Single-Sensor Light Response Functions";

// =========================== LRFdata 4Vector for Data Management ===========================
    py::class_<LRFdata>(m,"LRFdata")
        .def(py::init<double, double, double, double>())
        .def_readwrite("x",&LRFdata::x)
        .def_readwrite("y",&LRFdata::y)
        .def_readwrite("z",&LRFdata::z)
        .def_readwrite("val",&LRFdata::val)
        ;

// =========================== LRF (General Class -- Factory Function) ===========================
    //py::class_<LRF>(m,"LRF")
    //    .def_static("fromJsonString", &LRF::mkFromJsonString)
    //    ;
    
// =========================== LRFxyz (3D -- No constrained symmetry) ===========================
    py::class_<LRFxyz>(m, "LRFxyz", "LRF in 3 Dimensions. No assumed Symmetry.")
        .def(py::init<double, double, int, double, double, int, double, double, int>(),
            py::arg("xMin"), py::arg("xMax"), py::arg("nXBins"), py::arg("yMin"), py::arg("yMax"), py::arg("nYBins"), py::arg("zMin"), py::arg("zMax"), py::arg("nZBins"))
        .def(py::init<std::string &>())
        .def("GetJsonString", (std::string (LRFxyz::*)() const) &LRFxyz::GetJsonString)
        .def("inDomain", (bool (LRFxyz::*)(double x, double y, double z)) &LRFxyz::inDomain)
        .def("isValid", &LRFxyz::isValid)
        .def("isReady", &LRFxyz::isReady)
        .def("getRmax", &LRFxyz::getRmax)
        .def("getNintX", &LRFxyz::getNintX)
        .def("getNintY", &LRFxyz::getNintY)
        .def("getNintZ", &LRFxyz::getNintZ)
        .def("type", &LRFxyz::type)

    // Data Management & Fitting
        .def("addData", &LRFxyz::addData)
        .def("clearData", &LRFxyz::clearData)
        .def("doFit", &LRFxyz::doFit)
        .def("fitData", &LRFxyz::fitData)

    // Advanced Bin/Fitting Settings
        .def("SetNonNegative", (void (LRFxyz::*)(bool val)) &LRFxyz::SetNonNegative)
        .def("SetMinWeight", (void (LRFxyz::*)(double val)) &LRFxyz::SetMinWeight)
        .def("SetMissingFactor", (void (LRFxyz::*)(double val)) &LRFxyz::SetMissingFactor)

    // LRF Evaluation
        .def("eval", (double (LRFxyz::*) (double x, double y, double z)) &LRFxyz::eval)
        .def("evalDrvX", (double (LRFxyz::*) (double x, double y, double z)) &LRFxyz::evalDrvX)
        .def("evalDrvY", (double (LRFxyz::*) (double x, double y, double z)) &LRFxyz::evalDrvY)
        .def("evalDrvZ", (double (LRFxyz::*) (double x, double y, double z)) &LRFxyz::evalDrvZ)
        ;

// =========================== LRFxy (2D -- No constrained symmetry) ===========================
    py::class_<LRFxy>(m, "LRFxy", "LRF in 2 Dimensions. No assumed Symmetry.")
        .def(py::init<double, double, int, double, double, int>(),
        py::arg("xMin"), py::arg("xMax"), py::arg("nXBins"), py::arg("yMin"), py::arg("yMax"), py::arg("nYBins"))
        .def(py::init<std::string &>())
        .def("GetJsonString", (std::string (LRFxy::*)() const) &LRFxy::GetJsonString)
        .def("inDomain", (bool (LRFxy::*)(double x, double y, double z) const) &LRFxy::inDomain)
        .def("isValid", &LRFxy::isValid)
        .def("isReady", &LRFxy::isReady)
        .def("getRmax", &LRFxy::getRmax)
        .def("getNintX", &LRFxy::getNintX)
        .def("getNintY", &LRFxy::getNintY)
        .def("type", &LRFxy::type)

    // Data Management & Fitting
        .def("addData", &LRFxy::addData)
        .def("clearData", &LRFxy::clearData)
        .def("doFit", &LRFxy::doFit)
        .def("fitData", &LRFxy::fitData)
    
    // Advanced Bin/Fitting Settings
        .def("SetNonNegative", (void (LRFxy::*)(bool val)) &LRFxy::SetNonNegative)
        .def("SetTopDown", (void (LRFxy::*)(bool val, double x, double y)) &LRFxy::SetTopDown)

    // LRF Evaluation
        .def("eval", (double (LRFxy::*) (double x, double y, double z)) &LRFxy::eval)
        .def("evalDrvX", (double (LRFxy::*) (double x, double y, double z)) &LRFxy::evalDrvX)
        .def("evalDrvY", (double (LRFxy::*) (double x, double y, double z)) &LRFxy::evalDrvY)
        ;

// =========================== LRFaxial (2D -- Dimension Reduced by Axial Symmetry) ===========================
    py::class_<LRFaxial>(m, "LRFaxial", "LRF in 2 Dimensions. Assumed axial symmetry simplifies to one radial dimension.")
    // Constructors
        .def(py::init<double, double, double, int>(),
            py::arg("x0"), py::arg("y0"), py::arg("rmax"), py::arg("nint"))
        .def(py::init<std::string &>())
        
    // Clone
        .def("clone",
            [](const LRFaxial &self) {
                return self.clone();
            },
            py::return_value_policy::take_ownership)

    // Domain & readiness
        .def("inDomain", &LRFaxial::inDomain,
             py::arg("x"), py::arg("y"), py::arg("z") = 0.0)
        .def("isReady", &LRFaxial::isReady)

    // Parameters
        .def("getRmax", &LRFaxial::getRmax)
        .def("getNint", &LRFaxial::getNint)
        .def("GetNodes", &LRFaxial::GetNodes)

    // Evaluation
        .def("eval", &LRFaxial::eval,
             py::arg("x"), py::arg("y"), py::arg("z") = 0.0)
        .def("evalraw", &LRFaxial::evalraw,
             py::arg("x"), py::arg("y"), py::arg("z") = 0.0)
        .def("evalAxial", &LRFaxial::evalAxial)
        .def("evalDrvX", &LRFaxial::evalDrvX,
             py::arg("x"), py::arg("y"), py::arg("z") = 0.0)
        .def("evalDrvY", &LRFaxial::evalDrvY,
             py::arg("x"), py::arg("y"), py::arg("z") = 0.0)

    // Data fitting
        .def("fitData", &LRFaxial::fitData)
        .def("addData", &LRFaxial::addData)
        .def("doFit", &LRFaxial::doFit)
        .def("clearData", &LRFaxial::clearData)

    // Spline + JSON
        .def("type", &LRFaxial::type)
        .def("GetJsonString", (std::string (LRFaxial::*)() const) &LRFaxial::GetJsonString)

    // Setters
        .def("SetOrigin", &LRFaxial::SetOrigin)
        .def("SetRmin", &LRFaxial::SetRmin)
        .def("SetRmax", &LRFaxial::SetRmax)
        .def("SetFlatTop", &LRFaxial::SetFlatTop)
        .def("SetNonIncreasing", &LRFaxial::SetNonIncreasing)

    // Geometry
        .def("R", &LRFaxial::R)
        .def("R2", &LRFaxial::R2)

        .def("Rho", (double (LRFaxial::*) (double) const ) &LRFaxial::Rho)
        .def("Rho_xy", (double (LRFaxial::*) (double, double) const) &LRFaxial::Rho)
        .def("RhoDrvX", &LRFaxial::RhoDrvX)
        .def("RhoDrvY", &LRFaxial::RhoDrvY)

    // Getters
        .def("GetRmin", &LRFaxial::GetRmin)
        .def("GetOriginX", &LRFaxial::GetOriginX)
        .def("GetOriginY", &LRFaxial::GetOriginY)
        ;

}