#include "stepper.hpp"

#include <iostream>

std::function<double(double)> convert_string_to_fct(std::string fct) {};

template <typename T>
Stepper<T>::Stepper(Solver<T>& solver) {
    function = solver.get_function();
}

template <typename T>
Eigen::Vector2d Stepper<T>::aitken_step(Eigen::Vector2d last_iter, Eigen::Vector2d last_iter_two) {
    Eigen::Vector2d result;  // to complete
    return result;
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
