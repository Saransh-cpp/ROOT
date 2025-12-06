#ifndef ROOT_SOLVER_HPP
#define ROOT_SOLVER_HPP
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <functional>
#include <string>

#include "method.hpp"
#include "stepper.hpp"

template <typename T>
class Stepper;
/**
 * The Solver class will handle all the methods and arguments required for managing a solving process to find
 * the root of a Non-linear function.
 * For this scope, it will run a while loop and compute the error to check convergence, and will create an object
 * of the Stepper class to actually computing the step, passing the required arguments to it.
 * The results will be eventually stored in a Matrix, which will we passed to a Writer object to print it out in
 * a file or in the output window.
 */

/**
 * \brief Class managing the solving process
 */
template <typename T>
class Solver {
  private:
    Method method;
    /** \brief Stores the maximum iterations for the method*/
    int max_iterations;
    /** \brief Stores the tolerance below which the process ends*/
    double tolerance;
    /** \brief True if Aitken acceleration is required, 0 otherwise*/
    bool aitken_requirement;
    /** \brief Stores in the first column the points computed at each step, in
     * the second the value of the function at those points.
     */
    Eigen::MatrixX2d results;
    /** \brief Stores the function to find the root of and the starting guess for the process*/
    std::function<double(double)> function;
    T initial_guess;

  public:
    /** A complete constructor for the Solver class requires the info struct to have the function and the starting
     * guess, the maximum iterations to try, and the tolerance below which the process ends. If any of these inputs
     * are not given, default values (constexpr) will be assigned to them (max_iterations = 200, tolerance = 1e-6).
     * Potentially the aitken requirement could be set here, but for now we ask the user before the loop.
     */
    Solver(std::function<double(double)> fun, T initial_guess, const Method method, int max_iterations,
           double tolerance, bool aitken_mode);
    /** \brief Calls everything required to Solve with a method.*/
    /**
     * Caveat: could create and call the writer too.
     * Caveat: could add a method to restart the solver with a different method or whatever.
     */
    void loop(std::function<double(double)> additional_function = [](double x) { return x; });
    /** \brief Creates the stepper, calls the step computation, the error calculation and the results' saver.*/
    void while_body(int& iter, std::unique_ptr<Stepper<T>>& stepper, double& err);
    /** \brief Save the result of a step in a defined row of the results' matrix.*/
    void save_results(int iter, Eigen::Vector2d result_to_save);
    /** \brief Returns a row of the results matrix. @param step_length tells how far to go up from the bottom row */
    Eigen::Vector2d get_previous_result(int step_length);
    /** \brief Computes the error of the latest iteration computed*/
    double error_calculator(double x_prev, double x_next);
    /** \brief Saves the actual starting point in the top row of the results' matrix.
     *
     * It will be typed in the implementation to let the methods which require two starting points to store just
     * one of them.
     */
    void save_starting_point();
    /**
     * \brief Converts the generic Abstract stepper into a typed one. Right now according to user's input as string
     */
    void convert_stepper(
        std::unique_ptr<Stepper<T>>& stepper,
        std::function<double(double)> additional_function = [](double x) { return x; });
    /** \brief Returns the function argument of the class in order to correctly construct a Stepper object.*/
    std::function<double(double)> get_function();
    /** \brief Returns the Info argument of the class in order to use it in a Stepper object.*/
    int ask_next_action() const;
    void clear_results();
    void end_solver();
    bool ask_aitken();
    std::string ask_method();
    Solver<T>& operator=(const Solver<T>& solver);
};

#endif
