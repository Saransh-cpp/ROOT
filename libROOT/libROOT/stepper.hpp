#ifndef ROOT_STEPPER_HPP
#define ROOT_STEPPER_HPP

#include <Eigen/Dense>
#include <functional>
#include <iostream>

#include "stepper_def.hpp"

template <typename T>
StepperBase<T>::StepperBase(std::function<double(double)> fun, bool aitken_mode) {
    this->function = fun;
    this->aitken_requirement = aitken_mode;
}

template <typename T>
Eigen::Vector2d StepperBase<T>::step(Eigen::Vector2d previous_step) {
    if (!this->aitken_requirement) {
        return this->compute_step(previous_step);
    } else {
        return this->aitken_step(previous_step);
    }
}

template <typename T>
Eigen::Vector2d StepperBase<T>::aitken_step(Eigen::Vector2d previous_iter) {
    Eigen::Vector2d iter_one = this->compute_step(previous_iter);
    Eigen::Vector2d iter_two = this->compute_step(iter_one);
    double denominator = (iter_two(0) - iter_one(0)) / (iter_one(0) - previous_iter(0));
    if (denominator == 0) {
          std::cerr << "\033[31mCaught error: Division by 0. The method will diverge\033[0m" << std::endl;
      }
    double new_point = iter_two(0) - (pow(iter_two(0) - iter_one(0), 2) / denominator);
    return {new_point, function(new_point)};
}

template <>
NewtonRaphsonStepper<double>::NewtonRaphsonStepper(std::function<double(double)> fun, bool aitken_mode,
                                                   std::function<double(double)> der)
    : StepperBase<double>(fun, aitken_mode) {
    this->derivative = der;
}

template <>
Eigen::Vector2d NewtonRaphsonStepper<double>::compute_step(Eigen::Vector2d previous_iteration) {
    double denominator = derivative(previous_iteration(0));
    if (denominator == 0) {
          std::cerr << "\033[31mCaught error: Division by 0. The method will diverge\033[0m" << std::endl;
    }
    double new_point = previous_iteration(0) - previous_iteration(1) / denominator;
    double new_eval = this->function(new_point);
    return {new_point, new_eval};
}

template <>
FixedPointStepper<double>::FixedPointStepper(std::function<double(double)> fun, bool aitken_mode,
                                             std::function<double(double)> g_fun)
    : StepperBase<double>(fun, aitken_mode) {
    this->fixed_point_function = g_fun;
}

template <>
Eigen::Vector2d FixedPointStepper<double>::compute_step(Eigen::Vector2d previous_iteration) {
    double new_point = this->fixed_point_function(previous_iteration(0));
    double new_eval = this->function(new_point);
    return {new_point, new_eval};
}

template <>
ChordsStepper<Eigen::Vector2d>::ChordsStepper(std::function<double(double)> fun, bool aitken_mode, Eigen::Vector2d _int)
    : StepperBase<Eigen::Vector2d>(fun, aitken_mode) {
    auto interval = _int;
    this->iter_minus_1 = interval(0);
    this->iter_zero = interval(1);
}

template <>
Eigen::Vector2d ChordsStepper<Eigen::Vector2d>::compute_step(Eigen::Vector2d last_iter) {
    double numerator = this->iter_zero - this->iter_minus_1;
    double denominator = last_iter(1) - this->function(this->iter_minus_1);
    if (denominator == 0) {
        std::cerr << "\033[31mCaught error: Division by 0. The method will diverge\033[0m" << std::endl;
    }
    double new_point = this->iter_zero - last_iter(1) * numerator / denominator;
    this->iter_minus_1 = this->iter_zero;
    this->iter_zero = new_point;
    return {new_point, this->function(new_point)};
}
}

template <>
BisectionStepper<Eigen::Vector2d>::BisectionStepper(std::function<double(double)> fun, bool aitken_mode,
                                                    Eigen::Vector2d _int)
    : StepperBase<Eigen::Vector2d>(fun, aitken_mode) {
    auto interval = _int;
    this->left_edge = interval(0);
    this->right_edge = interval(1);
}

template <>
Eigen::Vector2d BisectionStepper<Eigen::Vector2d>::compute_step(Eigen::Vector2d last_iter) {
    double x_new = (this->left_edge + this->right_edge) / 2;
    if (this->function(x_new) * this->function(this->left_edge) < 0) {
        this->right_edge = x_new;
    } else {
        this->left_edge = x_new;
    }
    return {x_new, this->function(x_new)};
}

template class StepperBase<double>;
template class StepperBase<Eigen::Vector2d>;

template class NewtonRaphsonStepper<double>;
template class FixedPointStepper<double>;
template class BisectionStepper<Eigen::Vector2d>;
template class ChordsStepper<Eigen::Vector2d>;

#endif  // ROOT_STEPPER_HPP
