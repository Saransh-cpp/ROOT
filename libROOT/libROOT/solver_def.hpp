/**
 * @file solver_def.hpp
 * @brief Contains definition of class Solver to find the root of a non-linear equation with some numerical methods
 *
 * @author andreasaporito
 *
 * The Solver class will handle all the methods and arguments required for managing a solving process to find
 * the root of a Non-linear function.
 * For this scope, it will run a while loop and compute the error to check convergence, and will create an object
 * of the Stepper class to actually computing the step, passing the required arguments to it.
 * The results will be eventually stored in a Matrix, which will we passed to a Writer object to print it out in
 * a file or in the output window.
 */
#ifndef ROOT_SOLVER_DEF_HPP
#define ROOT_SOLVER_DEF_HPP
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <functional>
#include <string>

#include "method.hpp"
#include "stepper_def.hpp"

template <typename T>
class StepperBase;
/**
 * @brief Class Solver managing the solving process and creating the Stepper object
 */
template <typename T>
class Solver {
  private:
    Method method;            //!< Method which will be used - defined thanks to @author Saransh-ccp's Reader
    int max_iterations;       //!< Stores the maximum iterations for the method
    double tolerance;         //!< Stores the tolerance below which the process ends
    bool aitken_requirement;  //!< True if Aitken acceleration is required, 0 otherwise
    bool verbose;             //!< Verbose mode flag
    /** \brief Stores in the first column the points computed at each step, in
     * the second the value of the function at those points.
     */
    Eigen::MatrixX2d results;  //!< Stores in the first column the points computed at each step, in the second the value
                               //!< of the function at those points.
    std::function<double(double)>
        function;     //!< Stores the function to find the root of and the starting guess for the process
    T initial_guess;  //!< Templated initial_guess for the method, whose type changes depending on the method itself
    /** @brief Creates the stepper, calls the step computation, the error calculation and the results' saver.
     *
     * @param iter Reference to the current iteration, which will be increased once the step is computed
     * @param stepper The stepper object previously created and specialized depending on the method
     * @param err Reference to the error, which will be computed and updated to check convergence
     */
    void solver_step(int& iter, std::unique_ptr<StepperBase<T>>& stepper, double& err);
    /** @brief Saves the result of a step in a defined row of the results' matrix.
     *
     * @param iter The row index in which to store the result
     * @param result_to_save 2-dimensional vector storing the new guess x(i) and the evaluation at it f(x(i))
     */
    void save_results(int iter, Eigen::Vector2d result_to_save);
    /** @brief Returns a row of the results' matrix.
     *
     * @param step_length tells how far to go up from the bottom row
     * @return 2-dimensional vector storing x(end - step_length) and f(x(end - step_length))
     */
    Eigen::Vector2d get_previous_result(int step_length);
    /** @brief Computes the error of the latest iteration
     *
     * @param x_next The new guess x(i)
     * @param x_prev The old guess x(i-1)
     * @return |x_next - x_prev|
     */
    double calculate_error(double x_prev, double x_next);
    /** @brief Saves the actual initial guess in the top row of the results' matrix, no matter what type will be the
     * Class argument initial_guess.
     */
    void save_starting_point();
    /**
     * @brief Converts the generic Abstract stepper into a typed one
     *
     * @param stepper The original abstract stepper to be converted
     * @param additional_function An additional function which could be needed for certain methods
     */
    void convert_stepper(
        std::unique_ptr<StepperBase<T>>& stepper,
        std::function<double(double)> additional_function = [](double x) { return x; });

  public:
    /**
     * @brief Constructor for Solver object
     * @param fun The function to find the root of
     * @param initial_guess The initial guess(es) or interval
     * @param method The method which will be used for the Solution
     * @param max_iterations Maximum iterations in which the method has to converge
     * @param tolerance The tolerance below which the error/function will make the method converge
     * @param aitken_mode Option to apply Aitken's acceleration
     * @param verbose Option to give verbose output
     */
    Solver(std::function<double(double)> fun, T initial_guess, const Method method, int max_iterations,
           double tolerance, bool aitken_mode, bool verbose);
    /** @brief Calls everything required to Solve with a method.
     *
     * @return Matrix storing in the first column x(i) for each iteration i, in the second column f(x(i))
     */
    Eigen::MatrixX2d solve(std::function<double(double)> additional_function = [](double x) { return x; });
};

#endif  // ROOT_SOLVER_DEF_HPP
