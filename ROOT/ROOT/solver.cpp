#include "solver.hpp"

#include <iostream>

constexpr double tol = 1e-6;
constexpr int max_iters = 200;

template <typename T>
Solver<T>::Solver() = default;

template <typename T>
Solver<T>::Solver(Info<T> info, int max_iterations, double tolerance) {
    this->info = info;
    this->max_iterations = max_iterations;
    this->tolerance = tolerance;
}

template <typename T>
Solver<T>::Solver(Info<T> info, int max_iterations) {
    this->info = info;
    this->max_iterations = max_iterations;
    this->tolerance = tol;
}

template <typename T>
Solver<T>::Solver(Info<T> info, double tolerance) {
    this->info = info;
    this->tolerance = tolerance;
    this->max_iterations = max_iters;
}

template <typename T>
void Solver<T>::set_info(Info<T> info) {
    this->info = info;
}

template <typename T>
void Solver<T>::convert_stepper(auto stepper, std::string& method) {
    if (method == "newton") {
        stepper = std::make_unique<NewtonRaphsonStepper>(*this);
    } else if (method == "fixed point") {
        stepper = std::make_unique<FixedPointStepper>(*this);
    } else if (method == "bisection") {
        stepper = std::make_unique<BisectionStepper>(*this);
    } else if (method == "chords") {
        stepper = std::make_unique<ChordsStepper>(*this);
    }
}

template <typename T>
void Solver<T>::save_results(int iter, Eigen::Vector2d partial_result) {
    this->results.row(iter) = partial_result.transpose();
}

template <typename T>
Eigen::Vector2d Solver<T>::get_previous_result(int step_length) {
    return this->results.row(results.rows() - step_length);
}

template <>
void Solver<double>::save_starting_point() {
    save_results(0, {info.previous_iteration, info.function(info.previous_iteration)});
}

template <>
void Solver<Eigen::Vector2d>::save_starting_point() {
    save_results(0, {info.previous_iteration(0), info.function(info.previous_iteration(0))});
}

template <typename T>
double Solver<T>::error_calculator(double x_prev, double x_next) {
    return abs(x_prev - x_next);
}

template <typename T>
void Solver<T>::loop() {
    double err = 1;
    int iter = 0;
    std::string method;

    std::unique_ptr<Stepper<T>> stepper;  // not sure we really need this step

    std::cout << "Insert method, options: 'newton', 'fixed point', 'bisection', 'chords'" << std::endl;
    std::getline(std::cin, method);  // or could be saved in a Stepper<T> argument

    std::cout << "Do you want to apply the aitken acceleration? (1 for yes, 0 for no)" << std::endl;
    std::cin >> this->aitken_requirement;

    convert_stepper(stepper, method);

    save_starting_point();

    while (err > tolerance && iter < max_iterations) {
        while_body(&iter, stepper.get(), &err);
    }
}

template <typename T>
void Solver<T>::while_body(int& iter, auto stepper, double& err) {
    if (!this->aitken_requirement) {
        Eigen::Vector2d new_results = stepper->compute_step(this->get_previous_result(0));
        this->save_results(iter, new_results);
        err = error_calculator(new_results, this->get_previous_result(0)(0));
        iter++;
    } else {
        Eigen::Vector2d new_results = stepper->compute_step(this->get_previous_result(0));
        this->save_results(iter, new_results);
        iter++;
        new_results = stepper->compute_step(this->get_previous_result(0));
        this->save_results(iter, new_results);
        iter++;
        new_results = stepper->aitken_step(this->get_previous_result(0));
        this->save_results(iter, new_results);
        iter++;
        err = error_calculator(new_results, this->get_previous_result(0)(0));
    }
}
