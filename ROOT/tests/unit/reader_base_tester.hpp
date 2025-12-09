#ifndef READER_BASE_TESTER_HPP
#define READER_BASE_TESTER_HPP

#include <gtest/gtest.h>

#include "ROOT/reader.hpp"

/**
 * @brief Test fixture class for ReaderBase unit tests.
 *
 */
class ReaderBaseTester : public ::testing::Test {
  public:
    /**
     * @brief Test the trim method of ReaderBase.
     *
     * @param input The input string to be trimmed.
     * @param expected The expected trimmed string.
     */
    void testTrim(const std::string& input, const std::string& expected) {
        EXPECT_EQ(ReaderBase::trim(input), expected);
    }

    /**
     * @brief Test the parseBool method of ReaderBase.
     *
     * @param input The input string representing a boolean value.
     * @param expected The expected boolean value.
     */
    void testParseBool(const std::string& input, const bool& expected) {
        bool value;
        EXPECT_TRUE(ReaderBase::parseBool(input, value));
        EXPECT_EQ(value, expected);
    }

    /**
     * @brief Test the parseDouble method of ReaderBase.
     *
     * @param input The input string representing a double value.
     * @param expected The expected double value.
     */
    void testParseDouble(const std::string& input, const double& expected) {
        double value;
        EXPECT_TRUE(ReaderBase::parseDouble(input, value));
        EXPECT_DOUBLE_EQ(value, expected);
    }

    /**
     * @brief Test the parseInt method of ReaderBase.
     *
     * @param input The input string representing an integer value.
     * @param expected The expected integer value.
     */
    void testParseInt(const std::string& input, const int& expected) {
        int value;
        EXPECT_TRUE(ReaderBase::parseInt(input, value));
        EXPECT_EQ(value, expected);
    }

    /**
     * @brief Test the parseMethod method of ReaderBase.
     *
     * @param input The input string representing a Method.
     * @param expected The expected Method value.
     */
    void testParseMethod(const std::string& input, const Method& expected) {
        Method method;
        EXPECT_TRUE(ReaderBase::parseMethod(input, method));
        EXPECT_EQ(method, expected);
    }
};

#endif  // READER_BASE_TESTER_HPP
