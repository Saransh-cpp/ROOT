#ifndef SOLVER_TESTER_HPP
#define SOLVER_TESTER_HPP

#include <gtest/gtest.h>

#include <Eigen/Dense>
#include <libROOT/solver.hpp>
#include <libROOT/stepper.hpp>

class SolverTester : public ::testing::Test {
  public:
    template <typename T>
    void testConverStepper(std::function<double(double)> func, T initial_guess, Method method,
                           std::function<double(double)> derivative_or_function_g = nullptr) {
        Solver<T> solver(func, initial_guess, method, 100, 1e-6, false, false, derivative_or_function_g);
        std::unique_ptr<StepperBase<T>> stepper;
        solver.convert_stepper(stepper);
        ASSERT_NE(stepper, nullptr) << "Stepper conversion failed for method.";

        // checks to verify the type of the stepper
        switch (method) {
            case Method::NEWTON:
                if constexpr (std::is_same<T, double>::value) {
                    ASSERT_NE(dynamic_cast<NewtonRaphsonStepper<double>*>(stepper.get()), nullptr)
                        << "Stepper is not of type NewtonRaphsonStepper for scalar initial guess.";
                } else if constexpr (std::is_same<T, Eigen::Vector2d>::value) {
                    ASSERT_NE(dynamic_cast<NewtonRaphsonStepper<Eigen::Vector2d>*>(stepper.get()), nullptr)
                        << "Stepper is not of type NewtonRaphsonStepper for vector initial guess.";
                }
                break;
            case Method::FIXED_POINT:
                if constexpr (std::is_same<T, double>::value) {
                    ASSERT_NE(dynamic_cast<FixedPointStepper<double>*>(stepper.get()), nullptr)
                        << "Stepper is not of type FixedPointStepper for scalar initial guess.";
                } else if constexpr (std::is_same<T, Eigen::Vector2d>::value) {
                    ASSERT_NE(dynamic_cast<FixedPointStepper<Eigen::Vector2d>*>(stepper.get()), nullptr)
                        << "Stepper is not of type FixedPointStepper for vector initial guess.";
                }
                break;
            case Method::BISECTION:
                if constexpr (std::is_same<T, double>::value) {
                    ASSERT_NE(dynamic_cast<BisectionStepper<double>*>(stepper.get()), nullptr)
                        << "Stepper is not of type BisectionStepper for scalar initial guess.";
                }
                break;
            case Method::CHORDS:
                if constexpr (std::is_same<T, double>::value) {
                    ASSERT_NE(dynamic_cast<ChordsStepper<double>*>(stepper.get()), nullptr)
                        << "Stepper is not of type ChordsStepper for scalar initial guess.";
                }
                break;
            default:
                FAIL() << "Unknown method provided.";
        }
    }

    void testSaveResults() {
        auto func = [](double x) { return x * x - 2; };
        double initial_guess = 1.0;
        Solver<double> solver(func, initial_guess, Method::NEWTON, 100, 1e-6, false, false,
                              [](double x) { return 2 * x; });

        Eigen::Vector2d result_to_save;
        result_to_save << 1.41421, 0.0;
        solver.save_results(0, result_to_save);

        Eigen::Vector2d retrieved_result = solver.get_previous_result(0);
        ASSERT_DOUBLE_EQ(retrieved_result(0), result_to_save(0)) << "Saved and retrieved x values do not match.";
        ASSERT_DOUBLE_EQ(retrieved_result(1), result_to_save(1)) << "Saved and retrieved f(x) values do not match.";
    }

    void testCalculateError() {
        auto func = [](double x) { return x * x - 2; };
        double initial_guess = 1.0;
        Solver<double> solver(func, initial_guess, Method::NEWTON, 100, 1e-6, false, false,
                              [](double x) { return 2 * x; });

        double x_prev = 1.0;
        double x_next = 1.41421;
        double expected_error = std::abs(x_next - x_prev);
        double calculated_error = solver.calculate_error(x_prev, x_next);

        ASSERT_DOUBLE_EQ(calculated_error, expected_error) << "Calculated error does not match expected error.";
    }

    void testSaveStartingPoint() {
        auto func = [](double x) { return x * x - 2; };
        double initial_guess = 1.0;
        Solver<double> solver(func, initial_guess, Method::NEWTON, 100, 1e-6, false, false,
                              [](double x) { return 2 * x; });

        solver.save_starting_point();

        Eigen::Vector2d retrieved_result = solver.get_previous_result(solver.results.rows() - 1);
        ASSERT_DOUBLE_EQ(retrieved_result(0), initial_guess) << "Saved starting point x does not match initial guess.";
        ASSERT_DOUBLE_EQ(retrieved_result(1), func(initial_guess))
            << "Saved starting point f(x) does not match function evaluation at initial guess.";
    }

    void testGetPreviousResult() {
        auto func = [](double x) { return x * x - 2; };
        double initial_guess = 1.0;
        Solver<double> solver(func, initial_guess, Method::NEWTON, 100, 1e-6, false, false,
                              [](double x) { return 2 * x; });

        Eigen::Vector2d first_result;
        first_result << 1.41421, 0.0;
        solver.save_results(0, first_result);

        Eigen::Vector2d second_result;
        second_result << 1.73205, 0.0;
        solver.save_results(1, second_result);

        Eigen::Vector2d retrieved_first_result = solver.get_previous_result(1);
        ASSERT_DOUBLE_EQ(retrieved_first_result(0), first_result(0)) << "Retrieved previous x value does not match.";
        ASSERT_DOUBLE_EQ(retrieved_first_result(1), first_result(1)) << "Retrieved previous f(x) value does not match.";
    }

    template <typename T>
    void testSolverStep(std::function<double(double)> func, T initial_guess, Method method,
                        std::function<double(double)> derivative_or_function_g = nullptr) {
        Solver<T> solver(func, initial_guess, method, 100, 1e-6, false, false, derivative_or_function_g);
        std::unique_ptr<StepperBase<T>> stepper;
        solver.convert_stepper(stepper);
        ASSERT_NE(stepper, nullptr) << "Stepper conversion failed for method.";

        double err = 1;
        int iter = 1;

        solver.save_starting_point();
        solver.solver_step(iter, stepper, err);

        ASSERT_NE(err, 1) << "Error was not updated in solver_step.";
        ASSERT_EQ(iter, 2) << "Iteration count was not incremented in solver_step.";
        ASSERT_EQ(solver.results.rows(), 2) << "Results were not saved in solver_step.";
    }

    template <typename T>
    void testSolve(std::function<double(double)> func, T initial_guess, Method method,
                   std::function<double(double)> derivative_or_function_g = nullptr, int max_iterations = 100) {
        Solver<T> solver(func, initial_guess, method, max_iterations, 1e-6, false, false, derivative_or_function_g);
        solver.solve();

        ASSERT_GT(solver.results.rows(), 0) << "No results were recorded during solve.";
        double final_x = solver.results(solver.results.rows() - 1, 0);
        double final_fx = solver.results(solver.results.rows() - 1, 1);
        ASSERT_NEAR(final_fx, 0.0, 1e-4) << "Final function value is not close to zero.";
    }
};

#endif  // SOLVER_TESTER_HPP
