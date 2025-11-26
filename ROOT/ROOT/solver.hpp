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
    void while_body(int& iter, auto stepper, double& err);
    void save_results(int iter);                            // to do
    void get_previous_results(int step_length);             // to do
    double error_calculator(double x_prev, double x_next);  // to do -> convert to void via pointers
    void starting_point_setter(std::string& method);        // to do
    void starting_point_double(std::string& method);        // to do
    void starting_point_vector(std::string& method);        // to do
    void convert_stepper(auto stepper, std::string& method);
};

template <typename T>
class Stepper {
  public:
    Stepper(Solver<T>& solver);  // to do
    virtual void compute_step(int iter) = 0;
    void aitken_step(int iter);  // to do
};

class NewtonRaphsonStepper : public Stepper<double> {
  private:
    std::function<double(double)> derivative;

  public:
    NewtonRaphsonStepper(Solver<double>& solver);  // to do
    void set_derivative();                         // to do
    void compute_step(int iter) override;          // to do
};

class FixedPointStepper : public Stepper<double> {
  private:
    std::function<double(double)> fixed_point_function;

  public:
    FixedPointStepper(Solver<double>& solver);  // to do
    void set_fixed_point_function();            // to do
    void compute_step(int iter) override;       // to do
};

class ChordsStepper : public Stepper<Eigen::Vector2d> {
  private:
    double left_edge, right_edge;

  public:
    ChordsStepper(Solver<Eigen::Vector2d>& solver);  // to do
    void compute_step(int iter) override;            // to do
};

class BisectionStepper : public Stepper<Eigen::Vector2d> {
  private:
    double left_edge, right_edge;

  public:
    BisectionStepper(Solver<Eigen::Vector2d>& solver);  // to do
    void compute_step(int iter) override;               // to do
};

#endif
