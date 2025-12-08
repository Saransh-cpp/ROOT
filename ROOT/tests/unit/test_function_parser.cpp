#include <gtest/gtest.h>

#include "function_parser_base_tester.hpp"

TEST_F(FunctionParserBaseTester, IsPolynomial) {
    testIsPolynomial("x^2 + 3*x + 2", true);
    testIsPolynomial("sin(x) + x^2", false);
    testIsPolynomial("3*x^3 - 4*x + 1", true);
    testIsPolynomial("cos(x) - 5", false);
}

TEST_F(FunctionParserBaseTester, IsTrigonometric) {
    testIsTrigonometric("sin(x) + cos(x)", true);
    testIsTrigonometric("x^2 + 3*x + 2", false);
    testIsTrigonometric("2*sin(x) - 4*cos(x)", true);
    testIsTrigonometric("exp(x) + 5", false);
}

TEST_F(FunctionParserBaseTester, Icontains) {
    testIcontains("Hello World", "world", true);
    testIcontains("Function Parser", "parser", true);
    testIcontains("C++ Programming", "java", false);
    testIcontains("Unit Testing", "Test", true);
}

TEST_F(FunctionParserBaseTester, RemoveSpaces) {
    testRemoveSpaces("  Hello   World  ", "HelloWorld");
    testRemoveSpaces("Function   Parser", "FunctionParser");
    testRemoveSpaces(" C++  Programming ", "C++Programming");
    testRemoveSpaces(" Unit   Testing ", "UnitTesting");
}

TEST_F(FunctionParserBaseTester, SplitSignTokens) {
    testSplitSignTokens("+3*x^2 - 4*x + 5", {"+3*x^2 ", "- 4*x ", "+ 5"});
    testSplitSignTokens("-sin(x) + cos(x) - 2", {"-sin(x) ", "+ cos(x) ", "- 2"});
    testSplitSignTokens("+x^3 - x + 1", {"+x^3 ", "- x ", "+ 1"});
    testSplitSignTokens("-tan(x) + 4 - 3", {"-tan(x) ", "+ 4 ", "- 3"});
}

TEST_F(FunctionParserBaseTester, ParseOptionalCoefficient) {
    testParseOptionalCoefficient("3*x^2", {3.0, "x^2"});
    testParseOptionalCoefficient("-4.5*sin(x)", {-4.5, "sin(x)"});
    testParseOptionalCoefficient("+x^3", {1.0, "+x^3"});
    testParseOptionalCoefficient("-tan(x)", {1.0, "-tan(x)"});
}

TEST_F(FunctionParserBaseTester, ParseFunction) {
    // Test polynomial function
    {
        std::function<double(double)> expected = [](double x) { return 3 * x * x - 4 * x + 5; };
        testParseFunction("3*x^2 - 4*x + 5", expected, 2.0, 5e-6);
    }
    // Test trigonometric function
    {
        std::function<double(double)> expected = [](double x) { return -2 * sin(x) + 3 * cos(x); };
        testParseFunction("-2*sin(x) + 3*cos(x)", expected, 0.0, 5e-6);
    }
}
