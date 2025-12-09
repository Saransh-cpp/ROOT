#include <gtest/gtest.h>

#include "reader_base_tester.hpp"
#include "reader_csv_tester.hpp"

TEST_F(ReaderBaseTester, Trim) {
    testTrim(("  hello world  "), "hello world");
    testTrim(("\ttrim me\t"), "trim me");
    testTrim(("\nnew line\n"), "new line");
    testTrim(("   "), "");
    testTrim(("no_trim"), "no_trim");
}

TEST_F(ReaderBaseTester, BoolParsing) {
    testParseBool("true", true);
    testParseBool("false", false);
    testParseBool("1", true);
    testParseBool("0", false);
    testParseBool("yes", true);
    testParseBool("no", false);
    testParseBool("Y", true);
    testParseBool("n", false);
}

TEST_F(ReaderBaseTester, DoubleParsing) {
    testParseDouble("3.14", 3.14);
    testParseDouble("-2.71", -2.71);
    testParseDouble("0.0", 0.0);
}

TEST_F(ReaderBaseTester, IntParsing) {
    testParseInt("42", 42);
    testParseInt("-7", -7);
    testParseInt("0", 0);
}

TEST_F(ReaderBaseTester, MethodParsing) {
    testParseMethod("bisection", Method::BISECTION);
    testParseMethod("fixed_point", Method::FIXED_POINT);
    testParseMethod("newton", Method::NEWTON);
    testParseMethod("chords", Method::CHORDS);
}

TEST_F(ReaderCSVTester, SplitCsvLine) {
    testSplitCsvLine("value1,value2,value3", {"value1", "value2", "value3"});
    testSplitCsvLine("\"value, with, commas\",value2,\"value3\"", {"value, with, commas", "value2", "value3"});
    testSplitCsvLine("  value1  ,  value2  ,  value3  ", {"  value1  ", "  value2  ", "  value3  "});
    testSplitCsvLine("\"quoted \"\"value\"\"\",simple,value", {"quoted \"value\"", "simple", "value"});
    // with a different separator and quote character
    testSplitCsvLine("value1;value2;value3", {"value1", "value2", "value3"}, ';', '"');
    testSplitCsvLine("'value; with; semicolons';value2;'value3'", {"value; with; semicolons", "value2", "value3"}, ';',
                     '\'');
}
