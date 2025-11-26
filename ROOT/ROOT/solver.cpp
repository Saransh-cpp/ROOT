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
void Solver<T>::starting_point_setter(std::string& method) {}

template <>
void Solver<double>::starting_point_double(std::string& method) {}

template <>
void Solver<Eigen::Vector2d>::starting_point_vector(std::string& method) {}

template <typename T>
void Solver<T>::loop() {
    double err = 1;
    int iter = 0;
    std::string method;

    std::unique_ptr<Stepper<T>> stepper;

    std::cout << "Insert method, options: 'newton', 'fixed point', 'bisection', 'chords'" << std::endl;
    std::getline(std::cin, method);  // or could be saved in a Stepper<T> argument

    std::cout << "Do you want to apply the aitken acceleration? (1 for yes, 0 for no)" << std::endl;
    std::cin >> this->aitken_requirement;

    convert_stepper(stepper, method);

    starting_point_setter(method);

    while (err > tolerance && iter < max_iterations) {
        while_body(&iter, stepper.get(), &err);
    }
}

template <typename T>
void Solver<T>::while_body(int& iter, auto stepper, double& err) {
    if (!this->aitken_requirement) {
        stepper->compute_step(iter);
        err = error_calculator(this->results(iter - 1, 0), this->results(iter, 0));
        iter++;
    } else {
        stepper->compute_step(iter);
        iter++;
        stepper->compute_step(iter);
        iter++;
        stepper->aitken_step();
        iter++;
        err = error_calculator(this->results(iter - 1, 0), this->results(iter, 0));
    }
}
