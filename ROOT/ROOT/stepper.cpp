#include "stepper.hpp"

#include <iostream>

std::function<double(double)> convert_string_to_fct(std::string fct) {};

template <typename T>
Stepper<T>::Stepper(Solver<T>& solver) {
    function = solver.get_function();
}

template <typename T>
Eigen::Vector2d Stepper<T>::aitken_step(Eigen::Vector2d last_iter,
                                        Eigen::Vector2d last_iter_two,
                                        Eigen::Vector2d last_iter_three) {
    double denominator = (last_iter(0) - last_iter_two(0)) / (last_iter_two(0) - last_iter_three(0));
    double new_point = last_iter(0) - (pow(last_iter(0) - last_iter_two(0), 2) / denominator);
    return {new_point, function(new_point)};
}

NewtonRaphsonStepper::NewtonRaphsonStepper(Solver<double>& solver) : Stepper<double>(solver) {
    set_derivative();
}

void NewtonRaphsonStepper::set_derivative() {
    std::string der;
    std::cout << "Insert the derivative: " << std::endl;
    std::getline(std::cin, der);
    derivative = convert_string_to_fct(der);
}

Eigen::Vector2d NewtonRaphsonStepper::compute_step(Eigen::Vector2d previous_iteration) {
    double new_point = previous_iteration(0) - previous_iteration(1) / derivative(previous_iteration(0));
    double new_eval = function(new_point);
    return {new_point, new_eval};
}

FixedPointStepper::FixedPointStepper(Solver<double>& solver) : Stepper<double>(solver) {
    set_fixed_point_function();
}

void FixedPointStepper::set_fixed_point_function() {
    std::string fpf;
    std::cout << "Insert the fixed point function: " << std::endl;
    std::getline(std::cin, fpf);
    fixed_point_function = convert_string_to_fct(fpf);
}

Eigen::Vector2d FixedPointStepper::compute_step(Eigen::Vector2d previous_iteration) {
    double new_point = fixed_point_function(previous_iteration(0));
    double new_eval = function(new_point);
    return {new_point, new_eval};
}

ChordsStepper::ChordsStepper(Solver<Eigen::Vector2d>& solver) : Stepper<Eigen::Vector2d>(solver) {
    auto interval = solver.get_info();
    right_edge = interval();
}
