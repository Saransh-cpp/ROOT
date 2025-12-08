/**
 * @file stepper_def.hpp
 * @brief Contains definitions for all the Classes Stepper to compute steps of numerical methods to find the root
 * of non-linear equations
 *
 * @author andreasaporito
 *
 * The stepper classes will store the required functions, derivatives, and booleans to define how to compute the
 * step.
 * Each inherited class specializes the abstract Stepper to a real Stepper of a certain method.
 * The computation step is overridden in each class, and some additional arguments are saved depending on the
 * method. The generic Stepper constructor is inherited and the new specialized object will type the templated one.
 */
#ifndef ROOT_STEPPER_DEF_HPP
#define ROOT_STEPPER_DEF_HPP

#include <Eigen/Dense>
#include <functional>

/**
 * @brief The virtual mother stepper class which defines constructor and method in common for all the methods.
 */
template <typename T>
class StepperBase {
  protected:
    std::function<double(double)> function; //!< Function to compute the root of
    bool aitken_requirement; //!< Option to use Aitken's acceleration
    /**
     * @brief Virtual function to compute the step for the method -> overridden by all the methods
     */
    virtual Eigen::Vector2d compute_step(Eigen::Vector2d) = 0;
    /**
     * @brief Method to handle the computation of a step using Aitken's acceleration
     *
     * @param previous_iter 2-dimensional vector storing x(i-1) and f(x(i-1))
     * @return 2-dimensional vector storing x(i) - computed with the 3 Aitken's steps - and f(x(i))
     */
    Eigen::Vector2d aitken_step(Eigen::Vector2d previous_iter);

  public:
    /**
     * @brief Constructor for virtual Stepper class, which will be inherited by the daughters
     *
     * @param fun Function to compute the root of
     * @param aitken_mode Option to apply or not the Aitken's acceleration
     */
    StepperBase(std::function<double(double)> fun, bool aitken_mode);
    virtual ~StepperBase() = default;
    /**
     * @brief Method handling all the steps involved in computing the new guess
     *
     * @param previous_step 2-dimensional vector storing x(i-1) and f(x(i-1)) previous guesses of the method
     * @return 2-dimensional vector storing x(i) and f(x(i)) new guesses of the method
     */
    Eigen::Vector2d step(Eigen::Vector2d previous_step);
};

/**
 * @brief The specialized Stepper to compute a step with the Newton-Raphson method.
 */
template <typename T>
class NewtonRaphsonStepper : public StepperBase<T> {
  private:
    std::function<double(double)> derivative; //!< Stores the derivative of the function

public:
    /** @brief The specialized constructor - initializes the function and the derivative
     *
     * @param fun The function to compute the root of
     * @param aitken_mode Option to use Aitken's acceleration
     * @param der The derivative of the function, needed for NR method
     */
    NewtonRaphsonStepper(std::function<double(double)> fun, bool aitken_mode, std::function<double(double)> der);
    /** @brief Specialized method to compute and return a new step with NR
     *
     * @param previous_iteration 2-dimensional vector storing x(i-1) and f(x(i-1)) - previous guesses
     * @returns 2-dimensional vector storing x(i) = x(i-1) - f(x(i-1)) / f'(x(i-1)) and f(x(i)) - new guesses
     */
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

/** @brief The specialized Stepper to compute a step with the Fixed Point method*/
template <typename T>
class FixedPointStepper : public StepperBase<T> {
  private:
    std::function<double(double)> fixed_point_function; //!< tores the fixed point to use in the steps

  public:
    /** @brief The specialized constructor - initializes the function and the fixed point function
     *
     * @param fun The function to compute the root of
     * @param aitken_mode Option to use Aitken's acceleration
     * @param g_fun The fixed point function such that g_fun(x) = x, needed for FP method
     */
    FixedPointStepper(std::function<double(double)> fun, bool aitken_mode, std::function<double(double)> g_fun);
    /**
     * @brief Specialized method to compute and return a new step with FP
     *
     * @param previous_iteration 2-dimensional vector storing x(i-1) and f(x(i-1)) - previous guesses
     * @return 2-dimensional vector storing x(i) = g_fun(x(i-1)) - where g_fun is the fixed point function - and f(x(i))
     */
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

/** \brief The specialized Stepper to compute a step with the Chords Method (also called Secants in literature)*/
template <typename T>
class ChordsStepper : public StepperBase<T> {
  private:
    double iter_minus_1, iter_zero; //!< The two previous guesses required at each iteration
    // It could seem a bit redundant because the latest iteration is input in the compute_step method too, passed from
    // the Solver class, but it was more important to unify the compute_step calling with just one syntax.
  public:
    /** @brief Constructor for the ChordsStepper class
     *
     * @param fun The function to compute the root of
     * @param aitken_mode Option to use Aitken's acceleration
     * @param _int 2-dimensional vector storing the two initial guesses x(-1) and x(0)
     */
    ChordsStepper(std::function<double(double)> fun, bool aitken_mode, Eigen::Vector2d _int);
    /** @brief Specialized method to compute and return a new step with Chords.
     *
     * After the computation, the two previous guesses are then updated for the next step.
     *
     * @param previous_iteration 2-dimensional vector storing x(i-1) and f(x(i-1)) - old guesses
     * @return 2-dimensional vector storing x(i) = x(i-1) - (x(i-1) - x(i-2)) / (f(x(i-1)) - f(x(i-2))) * f(x(i-1)) and f(x(i)) - new guesses
     */
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

/** @brief The specialized Stepper to compute a step with the Bisection Method*/
template <typename T>
class BisectionStepper : public StepperBase<T> {
  private:
    double left_edge, right_edge; //!< Bounds of the interval to use (updated at each step)

  public:
    /** @brief Constructor of a BisectionStepper object
     *
     * @param fun The function to find the root of
     * @param aitken_mode Option to use Aitken's acceleration
     * @param _int Initial interval such that f(_int(0))*f(_int(1)) < 0
     */
    BisectionStepper(std::function<double(double)> fun, bool aitken_mode, Eigen::Vector2d _int);
    /** @brief Specialized method to compute and return a new step with Bisection.
     * Let left_edge = a, right_edge = b; then we have an interval [a,b] such that f(a)*f(b) < 0; We compute x_new = (a+b)/2; if f(a)*f(x_new) < 0 then
     * a_new = a, b_new = x_new, otherwise a_new = x_new, b_new = b -> left_edge = a_new, right_edge = b_new.
     *
     * @param previous_iteration 2-dimensional vector storing x(i-1) and f(x(i-1)) - old guesses
     * @return 2-dimensional vector storing x(i) = (left_edge + right_edge) / 2 and f(x(i)) - new guesses
     */
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

#endif  // ROOT_STEPPER_DEF_HPP
