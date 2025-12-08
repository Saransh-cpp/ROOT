#include "ROOT/reader.hpp"
#include "gtest/gtest.h"

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
    void testSplitCsvLine(const std::string& line, const std::vector<std::string>& expected) {
        ReaderCSV reader;
        std::vector<std::string> result = reader.splitCsvLine(line);
        EXPECT_EQ(result, expected);
    }
    void testReadCsv(const std::string& filename, char sep, char quote, bool has_header,
                     const std::unordered_map<std::string, std::string>& expected_config_map) {
        CLI::App app;
        auto* csv_subcommand = app.add_subcommand("csv");
        csv_subcommand->add_option("--file", filename)->required();
        csv_subcommand->add_option("--sep", sep)->capture_default_str();
        csv_subcommand->add_option("--quote", quote)->capture_default_str();
        csv_subcommand->add_option("--header", has_header)->capture_default_str();

        ReaderCSV reader;
        std::unique_ptr<ConfigBase> config = reader.read(csv_subcommand, false);
        ASSERT_NE(config, nullptr);

        // Convert ConfigBase back to map for comparison
        std::unordered_map<std::string, std::string> config_map;
        // (Implementation of conversion from ConfigBase to config_map is omitted for brevity)

        EXPECT_EQ(config_map, expected_config_map);
    }
};
