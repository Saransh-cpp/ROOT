#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <functional>

enum class Method { BISECTION, NEWTON, SECANT, FIXED_POINT };

class ConfigBase {
  public:
    Method method;
    double tolerance;
    int max_iterations;
    bool aitken;
    std::function<double(double)> function;
};

class BisectionConfig : public ConfigBase {
  public:
    double initial_point;
    double final_point;
    BisectionConfig(double tolerance, int max_iterations, bool aitken, std::function<double(double)> function,
                    double initial_point, double final_point) {
        this->tolerance = tolerance;
        this->max_iterations = max_iterations;
        this->aitken = aitken;
        this->function = function;
        this->initial_point = initial_point;
        this->final_point = final_point;
        this->method = Method::BISECTION;
    }
};

class NewtonConfig : public ConfigBase {
  public:
    double initial_guess;
    std::function<double(double)> derivative;
    NewtonConfig(double tolerance, int max_iterations, bool aitken, std::function<double(double)> function,
                 std::function<double(double)> derivative, double initial_guess) {
        this->tolerance = tolerance;
        this->max_iterations = max_iterations;
        this->aitken = aitken;
        this->function = function;
        this->derivative = derivative;
        this->initial_guess = initial_guess;
        this->method = Method::NEWTON;
    }
};

class SecantConfig : public ConfigBase {
  public:
    double initial_point;
    double final_point;
    SecantConfig();
    SecantConfig(double tolerance, int max_iterations, bool aitken, std::function<double(double)> function,
                 double initial_point, double final_point) {
        this->tolerance = tolerance;
        this->max_iterations = max_iterations;
        this->aitken = aitken;
        this->function = function;
        this->initial_point = initial_point;
        this->final_point = final_point;
        this->method = Method::SECANT;
    }
};

class FixedPointConfig : public ConfigBase {
  public:
    double initial_guess;
    std::function<double(double)> g_function;
    FixedPointConfig(double tolerance, int max_iterations, bool aitken, std::function<double(double)> function,
                     double initial_guess, std::function<double(double)> g_function) {
        this->tolerance = tolerance;
        this->max_iterations = max_iterations;
        this->aitken = aitken;
        this->function = function;
        this->initial_guess = initial_guess;
        this->g_function = g_function;
        this->method = Method::FIXED_POINT;
    }
};

#endif  // CONFIG_HPP
