#include "solver.hpp"
#include <iostream>
#include <memory>
#include "stepper.hpp"
#include "writer.hpp"

constexpr double tol = 1e-6;
constexpr int max_iters = 200;

template <typename T>
Solver<T>::Solver() = default;

template <typename T>
Info<T> read_info_from_user() {
    Info<T> info;
    return info;
}

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
void Solver<T>::convert_stepper(std::unique_ptr<Stepper<T>>& stepper, const std::string& method) {
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
void Solver<T>::save_results(int iter, Eigen::Vector2d result_to_save) {
    this->results.row(iter) = result_to_save.transpose();
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
    save_results(0, {info.previous_iteration(1), info.function(info.previous_iteration(1))});
}

template <typename T>
double Solver<T>::error_calculator(double x_prev, double x_next) {
    return abs(x_prev - x_next);
}

template <typename T>
std::function<double(double)> Solver<T>::get_function() {
    return info.function;
}

template <typename T>
Info<T> Solver<T>::get_info() {
    return info;
}

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

    save_starting_point();

    while (err > tolerance && iter < max_iterations) {
        while_body(&iter, stepper.get(), &err);
    }

    auto writer = std::make_unique<Writer<Eigen::MatrixX2d>>(results);
    writer->writing_process();

    end_solver();
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
        new_results = stepper->aitken_step(new_results, this->get_previous_result(1));
        this->save_results(iter, new_results);
        iter++;
        err = error_calculator(new_results, this->get_previous_result(0)(0));
    }
}

template <typename T>
int Solver<T>::ask_next_action() const {
    std::cout << "\nWhat would you like to do now?\n"
              << "0 - Exit program\n"
              << "1 - Restart solver with same Info\n"
              << "2 - Restart solver with NEW Info\n";

    int choice = 0;
    std::cin >> choice;

    return choice;
}

template <typename T>
void Solver<T>::clear_results() {
    results.resize(0, 0);
}

template <typename T>
void Solver<T>::end_solver() {
    int action = ask_next_action();

    if (action == 0) {
        std::cout << "Exiting program.\n";
        clear_results();
        return;
    }

    if (action == 1) {
        std::cout << "Restarting with same Info. \n";
        clear_results();
        loop();
    }

    if (action == 2) {
        std::cout << "Restarting solver with new Info. \n";
        info = read_info_from_user<T>();
        results.resize(0, 0);
        loop();
    }
}
