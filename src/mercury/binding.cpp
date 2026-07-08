#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "axial.h"
#include "formula1.h"
#include "formulav.h"
#include "formulaxy.h"
#include "lrm.h"
#include "rec.h"
#include "lrf.h"

namespace py = pybind11;
using namespace pybind11::literals; 

PYBIND11_MODULE(_mercury, m) {

    auto m_formula = m.def_submodule("formula", "formula");
    init_formula1(m_formula);
    init_formulav(m_formula);
    init_formulaxy(m_formula);

    auto m_lrm = m.def_submodule("lrm", "lrm");
    init_lrm(m_lrm);

    auto m_rec = m.def_submodule("rec", "rec");
    init_rec(m_rec);

    auto m_lrf = m.def_submodule("lrf", "lrf");
    init_lrf(m_lrf);

}