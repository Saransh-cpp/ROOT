#ifndef POLYNOMIAL_PARSER_TESTER_HPP
#define POLYNOMIAL_PARSER_TESTER_HPP

#include <gtest/gtest.h>

#include "ROOT/function_parser.hpp"

/**
 * @brief Test fixture class for PolynomialParser unit tests.
 *
 */
class PolynomialParserTester : public ::testing::Test {
  public:
    /**
     * @brief Test the parseTokenAsPolyTerm method of PolynomialParser.
     *
     * @param input The input token string to be parsed.
     * @param expected A std::function<double(double)> representing the expected parsed term.
     */
    void testParseTokenAsPolyTerm(const std::string& input, const std::function<double(double)>& expected) {
        std::function<double(double)> result;
        bool success = PolynomialParser::parseTokenAsPolyTerm(input, result);
        EXPECT_TRUE(success);

        // Test the resulting function at a few sample points
        for (double x : {-2.0, -1.0, 0.0, 1.0, 2.0}) {
            double expected_value = expected(x);
            double result_value = result(x);
            EXPECT_DOUBLE_EQ(result_value, expected_value);
        }
    }

    /**
     * @brief Test the parse method of PolynomialParser.
     *
     * @param input The input polynomial function string to be parsed.
     * @param expected A std::function<double(double)> representing the expected parsed polynomial function.
     */
    void testParse(const std::string& input, const std::function<double(double)>& expected) {
        PolynomialParser parser(input);
        std::function<double(double)> result = parser.parse();
        // Test the resulting function at a few sample points
        for (double x : {-2.0, -1.0, 0.0, 1.0, 2.0}) {
            double expected_value = expected(x);
            double result_value = result(x);
            EXPECT_DOUBLE_EQ(result_value, expected_value);
        }
    }
};

#endif  // POLYNOMIAL_PARSER_TESTER_HPP
