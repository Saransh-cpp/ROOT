#include <gtest/gtest.h>

#include "polynomial_parser_tester.hpp"

TEST_F(PolynomialParserTester, ParseTokenAsPolyTerm) {
    testParseTokenAsPolyTerm("3*x^2", [](double x) { return 3 * x * x; });
    testParseTokenAsPolyTerm("-4*x", [](double x) { return -4 * x; });
    testParseTokenAsPolyTerm("5", [](double) { return 5; });
    testParseTokenAsPolyTerm("x^3", [](double x) { return x * x * x; });
    testParseTokenAsPolyTerm("-2", [](double) { return -2; });
    testParseTokenAsPolyTerm("x", [](double x) { return x; });
}

TEST_F(PolynomialParserTester, ParsePolynomialFunction) {
    testParse("3*x^2 - 4*x + 5", [](double x) { return 3 * x * x - 4 * x + 5; });
    testParse("-x^3 + 2*x - 1", [](double x) { return -x * x * x + 2 * x - 1; });
    testParse("2*x^4 + 3*x^2 - x + 7", [](double x) { return 2 * x * x * x * x + 3 * x * x - x + 7; });
    testParse("x^5 - x^4 + x^3 - x^2 + x - 1",
              [](double x) { return x * x * x * x * x - x * x * x * x + x * x * x - x * x + x - 1; });
}
