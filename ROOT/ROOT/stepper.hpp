#ifndef ROOT_STEPPER_HPP
#define ROOT_STEPPER_HPP

#include <functional>
#include "solver.hpp"

template <typename T>
class Stepper {
  protected:
    std::function<double(double)> function;

  public:
    Stepper(Solver<T>& solver);
    virtual Eigen::Vector2d compute_step(Eigen::Vector2d) = 0;
    Eigen::Vector2d aitken_step(Eigen::Vector2d last_iter,
                                Eigen::Vector2d last_iter_two,
                                Eigen::Vector2d last_iter_three);
};

class NewtonRaphsonStepper : public Stepper<double> {
  private:
    std::function<double(double)> derivative;

  public:
    NewtonRaphsonStepper(Solver<double>& solver);
    void set_derivative();  // to do
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

class FixedPointStepper : public Stepper<double> {
  private:
    std::function<double(double)> fixed_point_function;

  public:
    FixedPointStepper(Solver<double>& solver);
    void set_fixed_point_function();  // to do
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

class ChordsStepper : public Stepper<Eigen::Vector2d> {
  private:
    double iter_minus_1, iter_zero;

  public:
    ChordsStepper(Solver<Eigen::Vector2d>& solver);
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

class BisectionStepper : public Stepper<Eigen::Vector2d> {
  private:
    double left_edge, right_edge;

  public:
    BisectionStepper(Solver<Eigen::Vector2d>& solver);
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

#endif  // ROOT_STEPPER_HPP
