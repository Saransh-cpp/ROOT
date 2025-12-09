#ifndef FUNCTION_PARSER_BASE_TESTER_HPP
#define FUNCTION_PARSER_BASE_TESTER_HPP

#include <gtest/gtest.h>

#include "ROOT/function_parser.hpp"

/**
 * @brief Test fixture class for FunctionParserBase unit tests.
 *
 */
class FunctionParserBaseTester : public ::testing::Test {
  public:
    /**
     * @brief Test the parseMethod method of ReaderBase.
     *
     * @param input The input string representing a Method.
     * @param expected The expected Method enum value.
     */
    void testIsPolynomial(const std::string& input, bool expected) {
        bool result = FunctionParserBase::isPolynomial(input);
        EXPECT_EQ(result, expected);
    }

    /**
     * @brief Test the isTrigonometric method of FunctionParserBase.
     *
     * @param input The input string representing a function.
     * @param expected The expected boolean result indicating if the function is trigonometric.
     */
    void testIsTrigonometric(const std::string& input, bool expected) {
        bool result = FunctionParserBase::isTrigonometric(input);
        EXPECT_EQ(result, expected);
    }

    /**
     * @brief Test the icontains method of FunctionParserBase.
     *
     * @param hay The string to search within.
     * @param needle The substring to search for.
     * @param expected The expected boolean result.
     */
    void testIcontains(const std::string& hay, const std::string& needle, bool expected) {
        bool result = FunctionParserBase::icontains(hay, needle);
        EXPECT_EQ(result, expected);
    }

    /**
     * @brief Test the removeSpaces method of FunctionParserBase.
     *
     * @param input The input string with potential spaces.
     * @param expected The expected string with spaces removed.
     */
    void testRemoveSpaces(const std::string& input, const std::string& expected) {
        std::string result = FunctionParserBase::removeSpaces(input);
        EXPECT_EQ(result, expected);
    }

    /**
     * @brief Test the splitSignTokens method of FunctionParserBase.
     *
     * @param input The input string to be split.
     * @param expected The expected vector of tokens after splitting.
     */
    void testSplitSignTokens(const std::string& input, const std::vector<std::string>& expected) {
        std::vector<std::string> result = FunctionParserBase::splitSignTokens(input);
        EXPECT_EQ(result, expected);
    }

    /**
     * @brief Test the parseOptionalCoefficient method of FunctionParserBase.
     *
     * @param input The input token string to parse.
     * @param expected The expected pair of coefficient and remaining string.
     */
    void testParseOptionalCoefficient(const std::string& input, const std::pair<double, std::string>& expected) {
        std::pair<double, std::string> result = FunctionParserBase::parseOptionalCoefficient(input);
        EXPECT_EQ(result, expected);
    }

    /**
     * @brief Test the parseFunction method of FunctionParserBase.
     *
     * @param input The input function string to parse.
     * @param expected The expected std::function<double(double)> result.
     * @param test_value A value to test the parsed function.
     * @param tolerance The acceptable tolerance for floating-point comparison.
     */
    void testParseFunction(const std::string& input, const std::function<double(double)>& expected, double test_value,
                           double tolerance) {
        std::function<double(double)> result = FunctionParserBase::parseFunction(input);
        double result_value = result(test_value);
        double expected_value = expected(test_value);
        EXPECT_NEAR(result_value, expected_value, tolerance);
    }
};

#endif  // FUNCTION_PARSER_BASE_TESTER_HPP
