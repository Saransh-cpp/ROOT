#include "function_parser.hpp"

#include <functional>
#include <string>

bool FunctionParserBase ::isPolynomial(const std::string& expression) {
    // Dummy check for polynomial expressions
    return expression.find("x") != std::string::npos && expression.find("sin") == std::string::npos &&
           expression.find("cos") == std::string::npos;
}
bool FunctionParserBase ::isTrigonometric(const std::string& expression) {
    // Dummy check for trigonometric expressions
    return expression.find("sin") != std::string::npos || expression.find("cos") != std::string::npos;
}

PolynomialParser ::PolynomialParser(std::string function_str) { this->function_str = function_str; }
std::function<double(double)> PolynomialParser ::parse() {
    // Dummy implementation for polynomial parsing
    this->function = [&](double var) {
        // Example: parse "x^2 + 2*x + 1"
        return (var * var) + (2 * var) + 1;  // Placeholder
    };
    return this->function;
}

TrigonometricParser ::TrigonometricParser(std::string function_str) { this->function_str = function_str; }
std::function<double(double)> TrigonometricParser ::parse() {
    // Dummy implementation for polynomial parsing
    this->function = [&](double var) {
        // Example: parse "x^2 + 2*x + 1"
        return (var * var) + (2 * var) + 1;  // Placeholder
    };
    return this->function;
}

std::function<double(double)> parseFunction(const std::string& function_str) {
    std::function<double(double)> parsed_function;
    if (FunctionParserBase::isPolynomial(function_str)) {
        PolynomialParser parser(function_str);
        parsed_function = parser.parse();
    } else if (FunctionParserBase::isTrigonometric(function_str)) {
        TrigonometricParser parser(function_str);
        parsed_function = parser.parse();
    } else {
        throw std::invalid_argument("Unsupported function type: " + function_str);
    }
    return parsed_function;
}
