/**
 * @file config.hpp
 * @brief Configuration (root) classes for root-finding methods.
 *
 * This file defines the configuration classes for various root-finding methods,
 * including Bisection, Newton, Secant, and Fixed Point methods. Each configuration
 * class encapsulates the parameters required for its respective method.
 *
 * @author Saransh-cpp
 *
 */
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <functional>
#include <libROOT/method.hpp>

/**
 * @brief Base (shared) configuration (data) class for root-finding methods.
 *
 * This class serves as the base for all specific root-finding method configurations,
 * encapsulating common parameters such as tolerance, maximum iterations, and the function.
 */
class ConfigBase {
  public:
    /**
     * @brief Virtual destructor required for downcasting objects to derived class types (using dynamic_cast).
     *
     */
    virtual ~ConfigBase() {};
    Method method;                           //!< The root-finding method to be used.
    double tolerance;                        //!< The tolerance for convergence.
    int max_iterations;                      //!< The maximum number of iterations allowed.
    bool aitken;                             //!< Indicates whether Aitken acceleration is enabled.
    bool verbose;                            //!< Indicates whether verbose output is enabled.
    std::function<double(double)> function;  //!< The function for which the root is to be found.
};

/**
 * @brief Configuration (data) class for the Bisection method.
 *
 * This class extends ConfigBase and includes specific parameters for the Bisection method,
 * such as the initial and final points of the interval.
 */
class BisectionConfig : public ConfigBase {
  public:
    double initial_point;  //!< The initial point of the interval.
    double final_point;    //!< The final point of the interval.
    /**
     * @brief Constructor for BisectionConfig.
     *
     * @param tolerance The tolerance for convergence.
     * @param max_iterations The maximum number of iterations allowed.
     * @param aitken Indicates whether Aitken acceleration is enabled.
     * @param function The function for which the root is to be found.
     * @param initial_point The initial point of the interval.
     * @param final_point The final point of the interval.
     */
    BisectionConfig(double tolerance, int max_iterations, bool aitken, std::function<double(double)> function,
                    double initial_point, double final_point, bool verbose) {
        this->tolerance = tolerance;
        this->max_iterations = max_iterations;
        this->aitken = aitken;
        this->function = function;
        this->initial_point = initial_point;
        this->final_point = final_point;
        this->method = Method::BISECTION;
        this->verbose = verbose;
        // validation
        double f_a = function(initial_point);
        double f_b = function(final_point);
        if (f_a * f_b > 0) {
            std::cerr << "ERROR: For Chords method, function values at initial points must have opposite signs.\n";
            std::cerr << "f(" << initial_point << ") = " << f_a << "\n";
            std::cerr << "f(" << final_point << ") = " << f_b << "\n";
            std::cerr << "Try different points and check whether the function positive everywhere.\n";
            std::cerr << "Program terminated.\n";
            //LLM
            std::exit(EXIT_FAILURE);
        }
    }
};

/**
 * @brief Configuration (data) class for the Newton method.
 *
 * This class extends ConfigBase and includes specific parameters for the Newton method,
 * such as the initial guess and the derivative of the function.
 */
class NewtonConfig : public ConfigBase {
  public:
    double initial_guess;                      //!< The initial guess for the root.
    std::function<double(double)> derivative;  //!< The derivative of the function.
    /**
     * @brief Constructor for NewtonConfig.
     *
     * @param tolerance The tolerance for convergence.
     * @param max_iterations The maximum number of iterations allowed.
     * @param aitken Indicates whether Aitken acceleration is enabled.
     * @param function The function for which the root is to be found.
     * @param derivative The derivative of the function.
     * @param initial_guess The initial guess for the root.
     */
    NewtonConfig(double tolerance, int max_iterations, bool aitken, std::function<double(double)> function,
                 std::function<double(double)> derivative, double initial_guess, bool verbose) {
        this->tolerance = tolerance;
        this->max_iterations = max_iterations;
        this->aitken = aitken;
        this->function = function;
        this->derivative = derivative;
        this->initial_guess = initial_guess;
        this->method = Method::NEWTON;
        this->verbose = verbose;
    }
};

/**
 * @brief Configuration (data) class for the Secant method.
 *
 * This class extends ConfigBase and includes specific parameters for the Secant method,
 * such as the initial and final points.
 */
class ChordsConfig : public ConfigBase {
  public:
    double initial_point1;  //!< The initial point of the interval.
    double initial_point2;    //!< The final point of the interval.
    /**
     * @brief Constructor for SecantConfig.
     *
     * @param tolerance The tolerance for convergence.
     * @param max_iterations The maximum number of iterations allowed.
     * @param aitken Indicates whether Aitken acceleration is enabled.
     * @param function The function for which the root is to be found.
     * @param initial_point The initial point of the interval.
     * @param final_point The final point of the interval.
     */
    ChordsConfig(double tolerance, int max_iterations, bool aitken, std::function<double(double)> function,
                 double initial_point, double final_point, bool verbose) {
        this->tolerance = tolerance;
        this->max_iterations = max_iterations;
        this->aitken = aitken;
        this->function = function;
        this->initial_point1 = initial_point;
        this->initial_point2 = final_point;
        this->method = Method::CHORDS;
        this->verbose = verbose;
    }
};

/**
 * @brief Configuration (data) class for the Fixed Point method.
 *
 * This class extends ConfigBase and includes specific parameters for the Fixed Point method,
 * such as the initial guess and the g function.
 */
class FixedPointConfig : public ConfigBase {
  public:
    double initial_guess;                      //!< The initial guess for the root.
    std::function<double(double)> g_function;  //!< The g function for the Fixed Point method.
    /**
     * @brief Constructor for FixedPointConfig.
     *
     * @param tolerance The tolerance for convergence.
     * @param max_iterations The maximum number of iterations allowed.
     * @param aitken Indicates whether Aitken acceleration is enabled.
     * @param function The function for which the root is to be found.
     * @param initial_guess The initial guess for the root.
     * @param g_function The g function for the Fixed Point method.
     */
    FixedPointConfig(double tolerance, int max_iterations, bool aitken, std::function<double(double)> function,
                     double initial_guess, std::function<double(double)> g_function, bool verbose) {
        this->tolerance = tolerance;
        this->max_iterations = max_iterations;
        this->aitken = aitken;
        this->function = function;
        this->initial_guess = initial_guess;
        this->g_function = g_function;
        this->method = Method::FIXED_POINT;
        this->verbose = verbose;
    }
};

#endif  // CONFIG_HPP
