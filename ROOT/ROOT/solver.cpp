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
void Solver<T>::loop() {
    double err = 1;
    int iter = 0;

    std::unique_ptr<Stepper<double>> stepper = create_stepper();

    while (err > tolerance && iter < max_iterations) {
        stepper->compute_step();
        err = error_calculator(this->results(iter - 1, 0), this->results(iter, 0));
        iter++;
    }
}
