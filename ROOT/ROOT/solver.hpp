#ifndef ROOT_SOLVER_HPP
#define ROOT_SOLVER_HPP
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <functional>

template <typename T>
struct Info {
    std::function<double(double)> function;
    T previous_iteration;
};

template <typename T>
class Stepper;

double error_calculator(double x_prev, double x_next);

template <typename T>
class Solver {
  protected:
    int max_iterations;
    double tolerance;
    bool aitken_requirement;
    Eigen::MatrixX2d results;
    Info<T> info;

  public:
    Solver();
    Solver(Info<T> info, int max_iterations, double tolerance);
    Solver(Info<T> info, int max_iterations);
    Solver(Info<T> info, double tolerance);
    void set_info(Info<T> info);
    void loop();
    void while_body(int& iter, auto stepper, double& err);
    void save_results(int iter, Eigen::Vector2d partial_result);
    Eigen::Vector2d get_previous_result(int step_length);
    double error_calculator(double x_prev, double x_next);
    void save_starting_point();
    void convert_stepper(auto stepper, const std::string& method);
    std::function<double(double)> get_function();
    Info<T> get_info();
};

#endif
