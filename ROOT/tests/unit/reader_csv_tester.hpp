#ifndef READER_CSV_TESTER_HPP
#define READER_CSV_TESTER_HPP

#include <gtest/gtest.h>

#include "ROOT/reader.hpp"

/**
 * @brief Test fixture class for ReaderBase unit tests.
 *
 */
class ReaderCSVTester : public ::testing::Test {
  public:
    /**
     * @brief Test the splitCsvLine method of ReaderCSV.
     *
     * @param line The input CSV line to be split.
     * @param expected The expected vector of strings after splitting.
     */
    void testSplitCsvLine(const std::string& line, const std::vector<std::string>& expected, const char sep = ',',
                          const char quote = '"') {
        ReaderCSV reader;
        reader.sep = sep;      // default separator
        reader.quote = quote;  // default quote character
        std::vector<std::string> result = reader.splitCsvLine(line);
        EXPECT_EQ(result, expected);
    }
};

#endif  // READER_CSV_TESTER_HPP
