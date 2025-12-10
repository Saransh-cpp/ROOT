#include <gtest/gtest.h>

#include "libROOT/solver.hpp"
#include "solver_tester.hpp"

TEST_F(SolverTester, ConvertStepperScalarNewton) {
    auto func = [](double x) { return x * x - 2; };
    double initial_guess = 1.0;
    this->testConverStepper<double>(func, initial_guess, Method::NEWTON, [](double x) { return 2 * x; });
}

TEST_F(SolverTester, ConvertStepperVectorBisection) {
    auto func = [](double x) { return x * x - 2; };
    Eigen::Vector2d initial_guess;
    initial_guess << 1.0, 2.0;
    this->testConverStepper<Eigen::Vector2d>(func, initial_guess, Method::BISECTION);
}

TEST_F(SolverTester, ConvertStepperScalarFixedPoint) {
    auto func = [](double x) { return cos(x); };
    double initial_guess = 0.5;
    this->testConverStepper<double>(func, initial_guess, Method::FIXED_POINT);
}

TEST_F(SolverTester, ConvertStepperVectorChords) {
    auto func = [](double x) { return x * x - 2; };
    Eigen::Vector2d initial_guess;
    initial_guess << 1.0, 2.0;
    this->testConverStepper<Eigen::Vector2d>(func, initial_guess, Method::CHORDS);
}

TEST_F(SolverTester, SaveResults) { this->testSaveResults(); }

TEST_F(SolverTester, CalculateError) { this->testCalculateError(); }

TEST_F(SolverTester, SaveStartingPoint) { this->testSaveStartingPoint(); }

TEST_F(SolverTester, GetPreviousResult) { this->testGetPreviousResult(); }

TEST_F(SolverTester, SolverStepNewtonScalar) {
    auto func = [](double x) { return x * x - 2; };
    double initial_guess = 1.0;
    this->testSolverStep<double>(func, initial_guess, Method::NEWTON, [](double x) { return 2 * x; });
}

TEST_F(SolverTester, SolverStepFixedPointScalar) {
    auto func = [](double x) { return x * x - x; };
    double initial_guess = 0.5;
    this->testSolverStep<double>(func, initial_guess, Method::FIXED_POINT, [](double x) { return x * x; });
}

TEST_F(SolverTester, SolverStepBisectionVector) {
    auto func = [](double x) { return x * x - 2; };
    Eigen::Vector2d initial_guess;
    initial_guess << 1.0, 2.0;
    this->testSolverStep<Eigen::Vector2d>(func, initial_guess, Method::BISECTION);
}

TEST_F(SolverTester, SolverStepChordsVector) {
    auto func = [](double x) { return x * x - 2; };
    Eigen::Vector2d initial_guess;
    initial_guess << 1.0, 2.0;
    this->testSolverStep<Eigen::Vector2d>(func, initial_guess, Method::CHORDS);
}

TEST_F(SolverTester, FullSolveNewtonScalar) {
    auto func = [](double x) { return x * x - 2; };
    double initial_guess = 1.0;
    this->testSolve<double>(func, initial_guess, Method::NEWTON, [](double x) { return 2 * x; });
}

TEST_F(SolverTester, FullSolveBisectionVector) {
    auto func = [](double x) { return x * x - 2; };
    Eigen::Vector2d initial_guess;
    initial_guess << 1.0, 2.0;
    this->testSolve<Eigen::Vector2d>(func, initial_guess, Method::BISECTION);
}

TEST_F(SolverTester, FullSolveFixedPointScalar) {
    auto func = [](double x) { return x * x - x; };
    double initial_guess = 0.5;
    this->testSolve<double>(func, initial_guess, Method::FIXED_POINT, [](double x) { return x * x; });
}

TEST_F(SolverTester, FullSolveChordsVector) {
    auto func = [](double x) { return x * x - 2; };
    Eigen::Vector2d initial_guess;
    initial_guess << 1.0, 2.0;
    this->testSolve<Eigen::Vector2d>(func, initial_guess, Method::CHORDS);
}
