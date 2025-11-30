#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <functional>
#include <string>

class FunctionParserBase {
  public:
    virtual std::function<double(double)> parse() = 0;

    // Static dispatch to appropriate subclass
    static std::function<double(double)> parseFunction(const std::string& function_str);

    // Type checks
    static bool isPolynomial(const std::string& expression);
    static bool isTrigonometric(const std::string& expression);

  protected:
    std::string function_str;
    // Shared helpers
    static bool icontains(const std::string& hay, const std::string& needle);
    static std::string removeSpaces(const std::string& function_str);
    static std::vector<std::string> splitSignTokens(const std::string& expr_no_ws);
    static std::pair<double, std::string> parseOptionalCoefficient(const std::string& token);
};

class PolynomialParser : public FunctionParserBase {
  public:
    PolynomialParser(std::string function_str);
    std::function<double(double)> parse() override;

  private:
    static bool parseTokenAsPolyTerm(const std::string& raw_token, std::function<double(double)>& out_term);
};

class TrigonometricParser : public FunctionParserBase {
  public:
    TrigonometricParser(std::string function_str);
    std::function<double(double)> parse() override;

  private:
    static bool parseTokenAsTrigTerm(const std::string& raw_token, std::function<double(double)>& out_term);
};

std::function<double(double)> parseFunction(const std::string& function_str);

#endif  // FUNCTION_HPP
