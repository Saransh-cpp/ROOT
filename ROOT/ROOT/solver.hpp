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
    std::function<double(double)> function;
    Info<T> info;

  public:
    Solver();
    Solver(Info<T> info, int max_iterations, double tolerance);
    Solver(Info<T> info, int max_iterations);
    Solver(Info<T> info, double tolerance);
    void set_info(Info<T> info);
    void loop();
    friend double error_calculator(double x_prev, double x_next);
    std::unique_ptr<Stepper<T>> create_stepper();
};

template <typename T>
class Stepper {
  public:
    Stepper(Solver<T>& solver);
    virtual void compute_step() = 0;
};

class NewtonRaphsonStepper : public Stepper<double> {
  private:
    std::function<double(double)> derivative;

  public:
    NewtonRaphsonStepper(Solver<double>& solver);
    void set_derivative();
    void compute_step() override;
};

class FixedPointStepper : public Stepper<double> {
  private:
    std::function<double(double)> fixed_point_function;

  public:
    FixedPointStepper(Solver<double>& solver);
    void set_fixed_point_function();
    void compute_step() override;
};

class ChordsStepper : public Stepper<Eigen::Vector2d> {
  private:
    double left_edge, right_edge;

  public:
    ChordsStepper(Solver<Eigen::Vector2d>& solver);
    void compute_step() override;
};

class BisectionStepper : public Stepper<Eigen::Vector2d> {
  private:
    double left_edge, right_edge;

  public:
    BisectionStepper(Solver<Eigen::Vector2d>& solver);
    void compute_step() override;
};

#endif
