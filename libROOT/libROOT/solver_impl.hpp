#ifndef ROOT_SOLVER_IMPL_HPP
#define ROOT_SOLVER_IMPL_HPP
#include <Eigen/Dense>
#include <iostream>
#include <memory>

#include "method.hpp"
#include "solver.hpp"
#include "stepper_impl.hpp"

constexpr double tol = 1e-6;
constexpr int max_iters = 200;

template <typename T>
Solver<T>::Solver(std::function<double(double)> fun, T initial_guess, const Method method, int max_iterations,
                  double tolerance, bool aitken_mode) {
    this->function = fun;
    this->initial_guess = initial_guess;
    this->method = method;
    this->max_iterations = max_iterations;
    this->tolerance = tolerance;
    this->results = Eigen::MatrixX2d(0, 2);
    this->aitken_requirement = aitken_mode;
}

// template <typename T>
// void convert_stepper(std::unique_ptr<Stepper<T>>& stepper, std::function<double(double)> additional_function =
// [](double x) {return x;});

template <>
void Solver<double>::convert_stepper(std::unique_ptr<Stepper<double>>& stepper,
                                     std::function<double(double)> additional_function) {
    if (method == Method::NEWTON)
        stepper = std::make_unique<NewtonRaphsonStepper<double>>(this->function, this->aitken_requirement,
                                                                 additional_function);
    if (method == Method::FIXED_POINT)
        stepper =
            std::make_unique<FixedPointStepper<double>>(this->function, this->aitken_requirement, additional_function);
}

template <>
void Solver<Eigen::Vector2d>::convert_stepper(std::unique_ptr<Stepper<Eigen::Vector2d>>& stepper,
                                              std::function<double(double)> additional_function) {
    if (method == Method::BISECTION) {
        stepper = std::make_unique<BisectionStepper<Eigen::Vector2d>>(this->function, this->aitken_requirement,
                                                                      this->initial_guess);
    }
    if (method == Method::SECANT)
        stepper = std::make_unique<ChordsStepper<Eigen::Vector2d>>(this->function, this->aitken_requirement,
                                                                   this->initial_guess);
}

template <typename T>
void Solver<T>::save_results(int iter, Eigen::Vector2d result_to_save) {
    results.conservativeResize(iter + 1, 2);
    this->results.row(iter) = result_to_save.transpose();
}

template <typename T>
Eigen::Vector2d Solver<T>::get_previous_result(int step_length) {
    int target_row = results.rows() - step_length - 1;
    return this->results.row(target_row);
}

template <>
void Solver<double>::save_starting_point() {
    if (results.rows() == 0) results.conservativeResize(1, 2);
    save_results(0, {initial_guess, function(initial_guess)});
}

template <>
void Solver<Eigen::Vector2d>::save_starting_point() {
    if (results.rows() == 0) results.resize(1, 2);
    double to_save = initial_guess(1);
    save_results(0, {to_save, function(to_save)});
}

template <typename T>
double Solver<T>::error_calculator(double x_prev, double x_next) {
    return abs(x_prev - x_next);
}

template <typename T>
std::function<double(double)> Solver<T>::get_function() {
    return function;
}

template <typename T>
Eigen::MatrixX2d Solver<T>::loop(std::function<double(double)> additional_function) {
    double err = 1.0;

    std::unique_ptr<Stepper<T>> stepper;

    convert_stepper(stepper, additional_function);

    save_starting_point();

    std::cout << "x(0): " << get_previous_result(0)(0) << "; f(x0): " << get_previous_result(0)(1) << std::endl;

    int iter = 1;

    while (err > tolerance && abs(get_previous_result(0)(1)) > tolerance && iter < max_iterations) {
        std::cout << "x(0): " << get_previous_result(0)(0) << "; f(x0): " << get_previous_result(0)(1) << std::endl;
        while_body(iter, stepper, err);
    }

    std::cout << "Exiting program." << std::endl;

    return results;
}

template <typename T>
void Solver<T>::while_body(int& iter, std::unique_ptr<Stepper<T>>& stepper, double& err) {
    std::cout << "Iteration " << iter << ": ";
    auto new_results = stepper->step(this->get_previous_result(0));
    save_results(iter, new_results);
    err = error_calculator(new_results(0), this->get_previous_result(1)(0));
    ++iter;
}

template class Solver<double>;
template class Solver<Eigen::Vector2d>;

#endif  // ROOT_SOLVER_IMPL_HPP
