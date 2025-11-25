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

template <>
void Solver<double>::loop() {
    double err = 1;
    int iter = 0;
    int type = 0;
    std::cout << "Insert 1 to use NR, 2 to use FP: " << std::endl;
    std::cin >> type;
    std::unique_ptr<Stepper<double>> stepper;
    if (type == 1) {
        auto temp_stepper = std::make_unique<NewtonRaphsonStepper>(*this);
        temp_stepper->set_derivative();
        stepper = std::move(temp_stepper);
    } else if (type == 2) {
        auto temp_stepper = std::make_unique<FixedPointStepper>(*this);
        temp_stepper->set_fixed_point_function();
        stepper = std::move(temp_stepper);
    } else {
        return;
    }
    while (err > tolerance && iter < max_iterations) {
        stepper->compute_step();
    }
}
