#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <functional>
#include <string>

/**
 * @brief Base class for function parsers.
 *
 * This abstract class defines the interface for parsing mathematical functions from strings.
 * It includes static methods for dispatching to specific parser subclasses based on the
 * type of function (e.g., polynomial, trigonometric).
 */
class FunctionParserBase {
  public:
    /**
     * @brief Pure virtual method to parse the function string.
     *
     * @return A std::function<double(double)> representing the parsed function.
     */
    virtual std::function<double(double)> parse() = 0;

    /**
     * @brief Static method to parse a function string and return a callable function.
     *
     * This method determines the type of function represented by the input string
     * and delegates parsing to the appropriate subclass parser.
     *
     * @param function_str The string representation of the function to be parsed.
     * @return A std::function<double(double)> representing the parsed function.
     */
    static std::function<double(double)> parseFunction(const std::string& function_str);

    /**
     * @brief Static method to check if the expression is a polynomial.
     *
     * @param expression The expression string to check.
     * @return true if the expression is a polynomial, false otherwise.
     */
    static bool isPolynomial(const std::string& expression);
    /**
     * @brief Static method to check if the expression is a trigonometric function.
     *
     * @param expression The expression string to check.
     * @return true if the expression is trigonometric, false otherwise.
     */
    static bool isTrigonometric(const std::string& expression);
    /**
     * @brief Constructor for FunctionParserBase.
     *
     * @param function_str The string representation of the function to be parsed.
     */
    FunctionParserBase(std::string function_str);

  protected:
    std::string function_str;  //!< The function string to be parsed.
    /**
     * @brief Helper static method to check if a string contains a substring (case-insensitive).
     *
     * @param hay The string to search within.
     * @param needle The substring to search for.
     * @return true if hay contains needle, false otherwise.
     */
    static bool icontains(const std::string& hay, const std::string& needle);
    /**
     * @brief Helper static method to remove all whitespace from a string.
     *
     * @param function_str The input string.
     * @return A new string with all whitespace removed.
     */
    static std::string removeSpaces(const std::string& function_str);
    /**
     * @brief Helper static method to split an expression into tokens based on sign characters.
     *
     * @param expr_no_ws The expression string with no whitespace.
     * @return A vector of tokens split by sign characters.
     */
    static std::vector<std::string> splitSignTokens(const std::string& expr_no_ws);
    /**
     * @brief Helper static method to parse an optional coefficient from a token.
     *
     * @param token The token string to parse.
     * @return A pair containing the coefficient and the remaining string.
     */
    static std::pair<double, std::string> parseOptionalCoefficient(const std::string& token);
};

/**
 * @brief Parser class for polynomial functions.
 *
 * This class extends FunctionParserBase and implements parsing logic specific to polynomial functions.
 */
class PolynomialParser : public FunctionParserBase {
  public:
    /**
     * @brief Constructor for PolynomialParser.
     *
     * @param function_str The string representation of the polynomial function to be parsed.
     */
    PolynomialParser(std::string function_str);
    /**
     * @brief Parse the polynomial function string.
     *
     * @return A std::function<double(double)> representing the parsed polynomial function.
     */
    std::function<double(double)> parse() override;

  private:
    /**
     * @brief Helper static method to parse a token as a polynomial term.
     *
     * @param raw_token The token string to parse.
     * @param out_term A reference to store the resulting polynomial term function.
     * @return true if the token was successfully parsed as a polynomial term, false otherwise.
     */
    static bool parseTokenAsPolyTerm(const std::string& raw_token, std::function<double(double)>& out_term);
};

/**
 * @brief Parser class for trigonometric functions.
 *
 * This class extends FunctionParserBase and implements parsing logic specific to trigonometric functions.
 */
class TrigonometricParser : public FunctionParserBase {
  public:
    /**
     * @brief Constructor for TrigonometricParser.
     *
     * @param function_str The string representation of the trigonometric function to be parsed.
     */
    TrigonometricParser(std::string function_str);
    /**
     * @brief Parse the trigonometric function string.
     *
     * @return A std::function<double(double)> representing the parsed trigonometric function.
     */
    std::function<double(double)> parse() override;

  private:
    /**
     * @brief Helper static method to parse a token as a trigonometric term.
     *
     * @param raw_token The token string to parse.
     * @param out_term A reference to store the resulting trigonometric term function.
     * @return true if the token was successfully parsed as a trigonometric term, false otherwise.
     */
    static bool parseTokenAsTrigTerm(const std::string& raw_token, std::function<double(double)>& out_term);
};

#endif  // FUNCTION_HPP
