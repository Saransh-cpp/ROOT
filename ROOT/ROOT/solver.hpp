#ifndef ROOT_SOLVER_HPP
#define ROOT_SOLVER_HPP
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <functional>

struct Info {
    std::function<double(double)> function;
    template <typename T>
    T previous_iteration;
};

class Solver {
  protected:
    static int max_iterations;
    static double tolerance;
    static bool aitken_requirement;
    Eigen::MatrixX2d results;
    std::function<double(double)> function;
    Info info;

  public:
    Solver();
    Solver(Info info, int max_iterations, double tolerance);
    Solver(Info info, int max_iterations);
    Solver(Info info, double tolerance);
    void get_info(Info info);
    void loop();
    friend double error_calculator(double x_prev, double x_next);
    friend class Stepper;
};

/**
 * I defined Stepper as a friend class and not a daughter class bc I just need it to be
 * allowed to access the results in the Solver, but it's not a Solver itself.
 * The results will be updated in the Stepper class for elegance, id est I don't want to
 * return two doubles and then save them in the looper function.
 */

class Stepper {
  protected:
    /**
     * Let's see if these arguments are useless bc I could directly access the last
     * row of results or directly with solver.info.previous_iteration.
     */
    double x_prev, x_next, f_prev, f_next;

  public:
    /**
     * Stepper is defined once per each Solver, and then its methods to step are called
     * at each iteration of the loop. When it's constructed, x_prev and f_prev will be
     * solver.starting_point and solver.function(x_prev)
     */
    Stepper(Solver solver);
}

class NewtonRaphsonStepper : public Stepper {
  private:
    std::function<double(double)> derivative;

  public:
    NewtonRaphsonStepper(Solver solver);
    friend void set_derivative();
    compute_guess_newton_raphson();
}

class FixedPointStepper : public Stepper {
  private:
    std::function<double(double)> fixed_point_function;

  public:
    FixedPointStepper(Solver solver, std::function<double(double)> fixed_point_function);
    void compute_guess_fixed_point();
}

class ChordsStepper : public Stepper {
  private:
    double left_edge, right_edge;

  public:
    ChordsStepper(Solver solver);
    void compute_guess_chords();
}

class BisectionStepper : public Stepper {
  private:
    double left_edge, right_edge;

  public:
    BisectionStepper(Solver solver);
    compute_guess_bisection();
}

#endif
