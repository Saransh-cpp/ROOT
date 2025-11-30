#include <Eigen/Dense>
#include "libROOT/root_lib.hpp"
#include "solver.hpp"
#include "stepper.hpp"
#include "writer.hpp"

int main() {
    Info<double> info;
    info.function = [](double x) { return x * x; };
    info.previous_iteration = 1.0;
    auto solver = Solver(info, 40, 1e-2);
    solver.loop();
    return 0;
}
