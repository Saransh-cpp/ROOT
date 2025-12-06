#ifndef ROOT_STEPPER_HPP
#define ROOT_STEPPER_HPP

#include <Eigen/Dense>
#include <functional>
/**
 * The stepper class is a mother class, in which the Stepper is just initialized, but not computed. Despite this, the
 * aitken step is in common for all the methods and its method is defined here as a consequence, and the same
 * applies for the function argument
 */

/**
 * \brief The stepper classes compute and return the results of an iteration requested by the Solver class
 */
template <typename T>
class Stepper {
  protected:
    /**	\brief function argument is got from the Solver class and is the function to compute the root of. */
    std::function<double(double)> function;
    bool aitken_requirement;

  public:
    Stepper(std::function<double(double)> fun, bool aitken_mode);
    virtual Eigen::Vector2d compute_step(Eigen::Vector2d) = 0;
    /** \brief Returns the result of the computation of the additional step which Aitken acceleration introduces.
     * The three parameters are the previous 3 steps of the iterative solution, required to compute the new one,
     * and are got from the Solver object which created the Stepper.
     */
    Eigen::Vector2d step(Eigen::Vector2d previous_step);
    Eigen::Vector2d aitken_step(Eigen::Vector2d previous_iter);
};

/**
 * Each inherited class specializes the abstract Stepper to a real Stepper of a certain method.
 * The computation step is overridden in each class, and some additional arguments are saved depending on the
 * method. The generic Stepper constructor is inherited and the new specialized object will type the templated one.
 */

/**
 * \brief The specialized Stepper to compute the root with the Newton-Raphson method.
 */
template <typename T>
class NewtonRaphsonStepper : public Stepper<T> {
  private:
    /** \brief Stores the derivative of the function, input by the user.*/
    std::function<double(double)> derivative;

  public:
    /** \brief The constructor initializes the function and the derivative*/
    NewtonRaphsonStepper(std::function<double(double)> fun, bool aitken_mode, std::function<double(double)> der);
    /** \brief x_new = x_old - f(x_old) / f'(x_old) */
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

/** \brief The specialized Stepper to compute the root with the Fixed Point method*/
template <typename T>
class FixedPointStepper : public Stepper<T> {
  private:
    /** \brief Stores the fixed point to use in the steps, input by the user.*/
    std::function<double(double)> fixed_point_function;

  public:
    /** \brief The constructor initializs the function and the derivative*/
    FixedPointStepper(std::function<double(double)> fun, bool aitken_mode, std::function<double(double)> g_fun);
    /** \brief x_new = phi(x_old), where phi is the fixed point function.*/
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

/** \brief The specialized Stepper to compute the root with the Chords Method (also called Secants in literature)*/
template <typename T>
class ChordsStepper : public Stepper<T> {
  private:
    /** \brief The method requires two precedent steps at each iteration, and they are saved and updated in these
     * arguments
     *
     * It could seem a bit redundant because the latest iteration is input in the compute_step method too, passed from
     * the Solver class, but it was more important to unify the compute_step calling with just one syntax.
     */
    double iter_minus_1, iter_zero;

  public:
    /** \brief The arguments are initialized from the Solver info*/
    ChordsStepper(std::function<double(double)> fun, bool aitken_mode, Eigen::Vector2d _int);
    /** \brief x_2 = x_1 - (x_1 - x_0) / (f(x_1) - f(x_0)) * f(x_1)
     *
     * Before the computation, the two arguments are then updated for the next step.
     */
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

/** \brief The specialized Stepper to compute the root with the Bisection Method*/
template <typename T>
class BisectionStepper : public Stepper<T> {
  private:
    /** \brief The method requires an interval which we save the bounds of */
    double left_edge, right_edge;

  public:
    /** The arguments are initialized by the Solver info*/
    BisectionStepper(std::function<double(double)> fun, bool aitken_mode, Eigen::Vector2d _int);
    /** \brief We have an interval [a,b] such that f(a)*f(b) < 0; We compute x_new = (a+b)/2; if f(a)*f(x_new) < 0 then
     * a_new = a, b_new = x_new, otherwise a_new = x_new, b_new = b.
     */
    Eigen::Vector2d compute_step(Eigen::Vector2d previous_iteration) override;
};

#include "stepper_impl.hpp"

#endif  // ROOT_STEPPER_HPP
