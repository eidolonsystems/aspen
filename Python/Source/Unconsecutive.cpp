#include "Aspen/Python/Unconsecutive.hpp"
#include "Aspen/Unconsecutive.hpp"
#include "Aspen/Python/Box.hpp"

using namespace Aspen;
using namespace pybind11;

void Aspen::export_unconsecutive(pybind11::module& module) {
  module.def("unconsecutive",
    [] (Box<object> series) {
      return Box(unconsecutive(std::move(series)));
    });
}