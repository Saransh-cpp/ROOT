#include "gtest/gtest.h"
#include "reader_tester.hpp"

TEST_F(ReaderTester, Trim) {
    testTrim(("  hello world  "), "hello world");
    testTrim(("\ttrim me\t"), "trim me");
    testTrim(("\nnew line\n"), "new line");
    testTrim(("   "), "");
    testTrim(("no_trim"), "no_trim");
}

TEST_F(ReaderTester, BoolParsing) {
    testParseBool("true", true);
    testParseBool("false", false);
    testParseBool("1", true);
    testParseBool("0", false);
    testParseBool("yes", true);
    testParseBool("no", false);
    testParseBool("Y", true);
    testParseBool("n", false);
}

TEST_F(ReaderTester, DoubleParsing) {
    testParseDouble("3.14", 3.14);
    testParseDouble("-2.71", -2.71);
    testParseDouble("0.0", 0.0);
}

TEST_F(ReaderTester, IntParsing) {
    testParseInt("42", 42);
    testParseInt("-7", -7);
    testParseInt("0", 0);
}

TEST_F(ReaderTester, MethodParsing) {
    testParseMethod("bisection", Method::BISECTION);
    testParseMethod("fixed_point", Method::FIXED_POINT);
    testParseMethod("newton", Method::NEWTON);
    testParseMethod("chords", Method::CHORDS);
}
