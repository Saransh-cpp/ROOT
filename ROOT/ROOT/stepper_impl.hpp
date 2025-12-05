#include "stepper.hpp"

#include <Eigen/Dense>
#include <functional>
#include <iostream>

std::function<double(double)> convert_string_to_fct(std::string fct) {
    return [](double x) { return 2 * x; };
};
template <>
void NewtonRaphsonStepper<double>::set_derivative();
template <>
void FixedPointStepper<double>::set_fixed_point_function();

template <typename T>
Stepper<T>::Stepper(std::function<double(double)> fun, bool aitken_mode) {
    function = fun;
    aitken_requirement = aitken_mode;
}

template <typename T>
Stepper<T>& Stepper<T>::operator=(const Stepper<T>& stepper) {
    function = stepper.function;
    return *this;
}

template <typename T>
Eigen::Vector2d Stepper<T>::step(Eigen::Vector2d previous_step) {
    if (!aitken_requirement) {compute_step(previous_step);}
    else {aitken_step(previous_step);}
}

template <typename T>
Eigen::Vector2d Stepper<T>::aitken_step(Eigen::Vector2d previous_iter) {
    Eigen::Vector2d iter_one = compute_step(previous_iter);
    Eigen::Vector2d iter_two = compute_step(iter_one);
    double denominator = (iter_two(0) - iter_one(0)) / (iter_one(0) - previous_iter(0));
    double new_point = iter_two(0) - (pow(iter_two(0) - iter_one(0), 2) / denominator);
    return {new_point, function(new_point)};
}

template <>
NewtonRaphsonStepper<double>::NewtonRaphsonStepper(std::function<double(double)> fun) : Stepper<double>(fun) {
    set_derivative();
}

template <>
void NewtonRaphsonStepper<double>::set_derivative() {
    std::string der;
    std::cout << "Insert the derivative: " << std::endl;
    std::getline(std::cin, der);
    derivative = convert_string_to_fct(der);
}

template <>
Eigen::Vector2d NewtonRaphsonStepper<double>::compute_step(Eigen::Vector2d previous_iteration) {
    double new_point = previous_iteration(0) - previous_iteration(1) / derivative(previous_iteration(0));
    double new_eval = function(new_point);
    return {new_point, new_eval};
}

template <>
FixedPointStepper<double>::FixedPointStepper(std::function<double(double)> fun) : Stepper<double>(fun) {
    set_fixed_point_function();
}

template <>
void FixedPointStepper<double>::set_fixed_point_function() {
    std::string fpf;
    std::cout << "Insert the fixed point function: " << std::endl;
    std::getline(std::cin, fpf);
    fixed_point_function = convert_string_to_fct(fpf);
}

template <>
Eigen::Vector2d FixedPointStepper<double>::compute_step(Eigen::Vector2d previous_iteration) {
    double new_point = fixed_point_function(previous_iteration(0));
    double new_eval = function(new_point);
    return {new_point, new_eval};
}

template <>
ChordsStepper<Eigen::Vector2d>::ChordsStepper(std::function<double(double)> fun, Eigen::Vector2d _int)
    : Stepper<Eigen::Vector2d>(fun) {
    auto interval = _int;
    iter_minus_1 = interval(0);
    iter_zero = interval(1);
}

template <>
Eigen::Vector2d ChordsStepper<Eigen::Vector2d>::compute_step(Eigen::Vector2d last_iter) {
    double numerator = iter_zero - iter_minus_1;
    double denominator = last_iter(1) - function(iter_minus_1);
    double new_point = iter_zero - last_iter(1) * numerator / denominator;
    iter_minus_1 = iter_zero;
    iter_zero = new_point;
    return {new_point, function(new_point)};
}

template <>
BisectionStepper<Eigen::Vector2d>::BisectionStepper(std::function<double(double)> fun, Eigen::Vector2d _int)
    : Stepper<Eigen::Vector2d>(fun) {
    auto interval = _int;
    left_edge = interval(0);
    right_edge = interval(1);
}

template <>
Eigen::Vector2d BisectionStepper<Eigen::Vector2d>::compute_step(Eigen::Vector2d last_iter) {
    double x_new = (left_edge + right_edge) / 2;
    if (function(x_new) * function(left_edge) < 0) {
        right_edge = x_new;
    } else {
        left_edge = x_new;
    }
    return {x_new, function(x_new)};
}

template class Stepper<double>;
template class Stepper<Eigen::Vector2d>;

template class NewtonRaphsonStepper<double>;
template class FixedPointStepper<double>;
template class BisectionStepper<Eigen::Vector2d>;
template class ChordsStepper<Eigen::Vector2d>;
