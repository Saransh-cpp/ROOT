#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <functional>
#include <string>

class FunctionParserBase {
  public:
    std::string function_str;
    std::function<double(double)> function;
    static inline bool isPolynomial(const std::string& expression);
    static inline bool isTrigonometric(const std::string& expression);
    virtual std::function<double(double)> parse() = 0;
};

class PolynomialParser : public FunctionParserBase {
  public:
    PolynomialParser(std::string function_str);
    std::function<double(double)> parse() override;
};

class TrigonometricParser : public FunctionParserBase {
  public:
    TrigonometricParser(std::string function_str);
    std::function<double(double)> parse() override;
};

std::function<double(double)> parseFunction(const std::string& function_str);

#endif  // FUNCTION_HPP
