#include <Eigen/Dense>
#include <iostream>
#include <memory>
#include "solver.hpp"
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
    this->results = Eigen::MatrixX2d(0, 2);
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
void convert_stepper(std::unique_ptr<Stepper<T>>& stepper, const std::string& method);

template <>
void Solver<double>::convert_stepper(std::unique_ptr<Stepper<double>>& stepper, const std::string& method) {
    if (method == "newton")
        stepper = std::make_unique<NewtonRaphsonStepper<double>>(this->info.function, this->aitken_requirement);
    if (method == "fixed point")
        stepper = std::make_unique<FixedPointStepper<double>>(this->info.function, this->aitken_requirement);
}

template <>
void Solver<Eigen::Vector2d>::convert_stepper(std::unique_ptr<Stepper<Eigen::Vector2d>>& stepper,
                                              const std::string& method) {
    if (method == "bisection")
        stepper =
            std::make_unique<BisectionStepper<Eigen::Vector2d>>(this->info.function, this->aitken_requirement ,this->info.previous_iteration);
    if (method == "chords")
        stepper = std::make_unique<ChordsStepper<Eigen::Vector2d>>(this->info.function, this->aitken_requirement, this->info.previous_iteration);
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
    if (results.rows() == 0)
        results.conservativeResize(1, 2);
    save_results(0, {info.previous_iteration, info.function(info.previous_iteration)});
}

template <>
void Solver<Eigen::Vector2d>::save_starting_point() {
    if (results.rows() == 0)
        results.resize(1, 2);
    double to_save = info.previous_iteration(1);
    save_results(0, {to_save, info.function(to_save)});
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
// used LLM for output readability
template <typename T>
bool Solver<T>::ask_aitken() {
    int choice;
    while (true) {
        std::cout << "Apply Aitken acceleration? (1 = yes, 0 = no): ";
        if (std::cin >> choice && (choice == 0 || choice == 1))
            return choice;

        std::cout << "Invalid input, please enter 0 or 1.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// LLm for clean output
template <typename T>
std::string Solver<T>::ask_method() {
    std::string method;
    std::vector<std::string> valid = {"newton", "fixed point", "bisection", "chords"};
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
        std::cout << "Insert method (newton, fixed point, bisection, chords): ";
        std::getline(std::cin, method);

        if (std::count(valid.begin(), valid.end(), method))
            return method;

        std::cout << "Invalid method. Try again.\n";
    }
}

template <typename T>
void Solver<T>::loop() {
    double err = 1.0;

    std::unique_ptr<Stepper<T>> stepper;

    std::string method = ask_method();
    aitken_requirement = ask_aitken();

    convert_stepper(stepper, method);

    save_starting_point();

    std::cout << "x(0): " << get_previous_result(0)(0) << "; f(x0): " << get_previous_result(0)(1) << std::endl;

    int iter = 1;

    while (err > tolerance && abs(get_previous_result(0)(1)) > tolerance && iter < max_iterations) {
        while_body(iter, stepper, err);
    }

    auto writer = std::make_unique<Writer<Eigen::MatrixX2d>>(results);
    writer->writing_process();

    end_solver();
}

template <typename T>
void Solver<T>::while_body(int& iter, std::unique_ptr<Stepper<T>>& stepper, double& err) {
    auto new_results = stepper->step(this->get_previous_result(0));
    save_results(iter, new_results);
    err = error_calculator(new_results(0), this->get_previous_result(1)(0));
    ++iter;
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
    results.resize(0, 2);
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

template <typename T>
Solver<T>& Solver<T>::operator=(const Solver<T>& solver) {
    if (this == &solver)
        return *this;
    max_iterations = solver.max_iterations;
    tolerance = solver.tolerance;
    aitken_requirement = solver.aitken_requirement;
    results = solver.results;
    info = solver.info;
    return *this;
}

template class Solver<double>;
template class Solver<Eigen::Vector2d>;
