#ifndef ROOT_SOLVER_HPP
#define ROOT_SOLVER_HPP
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <functional>

struct Info {
    std::function<double(double)> function;
    double starting_point;
};

class Solver {
  protected:
    static int max_iterations;
    static double tolerance;

  public:
    Solver();
    Solver(Info info, int max_iterations, double tolerance);
    Solver(Info info, int max_iterations);
    Solver(Info info, double tolerance);
};

#endif
