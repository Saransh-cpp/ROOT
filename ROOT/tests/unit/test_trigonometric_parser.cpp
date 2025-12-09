#include <gtest/gtest.h>

#include <cmath>

#include "trigonometric_parser_tester.hpp"

TEST_F(TrigonometricParserTester, ParseTokenAsTrigTerm) {
    testParseTokenAsTrigTerm("2*sin(x)", [](double x) { return 2 * std::sin(x); });
    testParseTokenAsTrigTerm("-3*cos(x)", [](double x) { return -3 * std::cos(x); });
    testParseTokenAsTrigTerm("sin(x)", [](double x) { return std::sin(x); });
    testParseTokenAsTrigTerm("-cos(x)", [](double x) { return -std::cos(x); });
    testParseTokenAsTrigTerm("4.5*sin(x)", [](double x) { return 4.5 * std::sin(x); });
    testParseTokenAsTrigTerm("-0.5*cos(x)", [](double x) { return -0.5 * std::cos(x); });
}

TEST_F(TrigonometricParserTester, ParseTrigonometricFunction) {
    testParse("2*sin(x) - 3*cos(x)", [](double x) { return 2 * std::sin(x) - 3 * std::cos(x); });
    testParse("-sin(x) + 4*cos(x)", [](double x) { return -std::sin(x) + 4 * std::cos(x); });
    testParse("5*sin(x) + cos(x)", [](double x) { return 5 * std::sin(x) + std::cos(x); });
    testParse("-2.5*sin(x) - 1.5*cos(x)", [](double x) { return -2.5 * std::sin(x) - 1.5 * std::cos(x); });
}
