#ifndef ROOT_SOLVER_HPP
#define ROOT_SOLVER_HPP
#include <Eigen/Dense>
#include <iostream>
#include <memory>

#include "method.hpp"
#include "solver_def.hpp"
#include "stepper.hpp"

constexpr double tol = 1e-6;
constexpr int max_iters = 200;

template <typename T>
Solver<T>::Solver(std::function<double(double)> fun, T initial_guess, const Method method, int max_iterations,
                  double tolerance, bool aitken_mode, bool verbose) {
    this->function = fun;
    this->initial_guess = initial_guess;
    this->method = method;
    this->max_iterations = max_iterations;
    this->tolerance = tolerance;
    this->results = Eigen::MatrixX2d(0, 2);
    this->aitken_requirement = aitken_mode;
    this->verbose = verbose;
}
template <typename T>
Solver<T>::Solver(std::function<double(double)> fun, T initial_guess, const Method method, int max_iterations,
                  double tolerance, bool aitken_mode, bool verbose,
                  std::function<double(double)> derivative_or_function_g) {
    this->function = fun;
    this->initial_guess = initial_guess;
    this->method = method;
    this->max_iterations = max_iterations;
    this->tolerance = tolerance;
    this->results = Eigen::MatrixX2d(0, 2);
    this->aitken_requirement = aitken_mode;
    this->verbose = verbose;
    this->derivative_or_function_g = derivative_or_function_g;
}

template <>
void Solver<double>::convert_stepper(std::unique_ptr<StepperBase<double>>& stepper) {
    switch (this->method) {
        case Method::NEWTON:
            stepper = std::make_unique<NewtonRaphsonStepper<double>>(this->function, this->aitken_requirement,
                                                                     this->derivative_or_function_g);
            break;
        case Method::FIXED_POINT:
            stepper = std::make_unique<FixedPointStepper<double>>(this->function, this->aitken_requirement,
                                                                  this->derivative_or_function_g);
            break;
        default:
            std::cerr << "\033[31mCaught error: Selected method is not compatible with scalar initial guess\033[0m"
                      << std::endl;
            break;
    }
}

template <>
void Solver<Eigen::Vector2d>::convert_stepper(std::unique_ptr<StepperBase<Eigen::Vector2d>>& stepper) {
    switch (this->method) {
        case Method::BISECTION:
            stepper = std::make_unique<BisectionStepper<Eigen::Vector2d>>(this->function, this->aitken_requirement,
                                                                          this->initial_guess);
            break;
        case Method::CHORDS:
            stepper = std::make_unique<ChordsStepper<Eigen::Vector2d>>(this->function, this->aitken_requirement,
                                                                       this->initial_guess);
            break;
        default:
            std::cerr << "\033[31mCaught error: Selected method is not compatible with vector initial guess\033[0m"
                      << std::endl;
            break;
    }
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
    this->save_results(0, {this->initial_guess, this->function(this->initial_guess)});
}

template <>
void Solver<Eigen::Vector2d>::save_starting_point() {
    if (results.rows() == 0) results.resize(1, 2);
    double to_save = initial_guess(1);
    this->save_results(0, {to_save, this->function(to_save)});
}

template <typename T>
double Solver<T>::calculate_error(double x_prev, double x_next) {
    return abs(x_prev - x_next);
}

template <typename T>
Eigen::MatrixX2d Solver<T>::solve() {
    double err = 1.0;

    std::unique_ptr<StepperBase<T>> stepper;

    convert_stepper(stepper);

    save_starting_point();

    if (this->verbose) {
        std::cout << "x(0): " << this->get_previous_result(0)(0) << "; f(x0): " << this->get_previous_result(0)(1)
                  << std::endl;
    }

    int iter = 1;

    while (err > this->tolerance && abs(this->get_previous_result(0)(1)) > this->tolerance &&
           iter < this->max_iterations) {
        if (this->verbose) {
            std::cout << "x(0): " << this->get_previous_result(0)(0) << "; f(x0): " << this->get_previous_result(0)(1)
                      << std::endl;
        }
        this->solver_step(iter, stepper, err);
    }

    if (iter == this->max_iterations && err > this->tolerance) {
        std::cerr << "033[31mThe solution did not converge in" << this->max_iterations << " iterations\033[0m"
                  << std::endl;
    }
    if (this->verbose) {
        if (err <= this->tolerance || abs(this->get_previous_result(0)(1)) <= this->tolerance) {
            std::cout << "Converged in " << iter - 1 << " iterations." << std::endl;
        }
    }
    std::cout << "Final estimate: x = " << this->get_previous_result(0)(0)
              << "; f(x) = " << this->get_previous_result(0)(1) << "; error = " << err << std::endl;

    return results;
}

template <typename T>
void Solver<T>::solver_step(int& iter, std::unique_ptr<StepperBase<T>>& stepper, double& err) {
    if (this->verbose) {
        std::cout << "Iteration " << iter << ": ";
    }
    auto new_results = stepper->step(this->get_previous_result(0));
    this->save_results(iter, new_results);
    err = this->calculate_error(new_results(0), this->get_previous_result(1)(0));
    ++iter;
}

template class Solver<double>;
template class Solver<Eigen::Vector2d>;

#endif  // ROOT_SOLVER_HPP
