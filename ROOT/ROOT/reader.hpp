/**
 * @file reader.hpp
 * @brief Reader classes for reading configuration from files.
 *
 * This file contains the definitions of ReaderBase, ReaderCSV, and ReaderDAT classes,
 * which are responsible for reading configuration data from various file formats
 * (e.g., CSV, DAT) and producing ConfigBase objects.
 *
 * @author Saransh-cpp
 */
#ifndef READER_HPP
#define READER_HPP

#include <CLI/CLI.hpp>
#include <memory>
#include <string>
#include <vector>

#include "config.hpp"

/**
 * @brief Base class for Reader classes.
 *
 * This abstract base class defines the interface for Reader classes that read configuration
 * data from various file formats (e.g., CSV, DAT) and produce ConfigBase objects.
 */
class ReaderBase {
  private:
    friend class ReaderBaseTester;  //!< For unit testing purposes.
  public:
    std::string filename;  //!< The input filename to read from.
    char sep;              //!< Field separator character.
    char quote;            //!< Quote/delimiter character.
    bool has_header;       //!< Indicates whether the first row is a header row.
    /**
     * @brief Virtual destructor for ReaderBase.
     *
     */
    virtual ~ReaderBase() = default;
    /**
     * @brief Read the configuration from the input.
     *
     * @param app The CLI app subcommand containing the options for a specific input type.
     * @return A unique pointer to a ConfigBase object representing the read configuration.
     */
    virtual std::unique_ptr<ConfigBase> read(CLI::App* app, bool verbose) = 0;

  protected:
    /**
     * @brief Helper static method to trim leading and trailing whitespace from a string.
     *
     * @param untrimmed_str The input string to be trimmed.
     * @return The trimmed string.
     */
    static std::string trim(const std::string& untrimmed_str);
    /**
     * @brief Helper static method to parse a boolean value from a string.
     *
     * @param bool_str The input string representing a boolean value.
     * @param out A reference to store the parsed boolean value.
     * @return true if parsing was successful, false otherwise.
     */
    static bool parseBool(const std::string& bool_str, bool& out);
    /**
     * @brief Helper static method to parse a double value from a string.
     *
     * @param double_str The input string representing a double value.
     * @param out A reference to store the parsed double value.
     * @return true if parsing was successful, false otherwise.
     */
    static bool parseDouble(const std::string& double_str, double& out);
    /**
     * @brief Helper static method to parse an integer value from a string.
     *
     * @param int_str The input string representing an integer value.
     * @param out A reference to store the parsed integer value.
     * @return true if parsing was successful, false otherwise.
     */
    static bool parseInt(const std::string& int_str, int& out);
    /**
     * @brief Helper static method to parse a Method enum value from a string.
     *
     * @param method_str The input string representing a Method.
     * @param out A reference to store the parsed Method value.
     * @return true if parsing was successful, false otherwise.
     */
    static bool parseMethod(const std::string& method_str, Method& out);
    /**
     * @brief Helper static method to create a ConfigBase object from a map of string key-value pairs.
     *
     * @param config_map The map containing configuration key-value pairs.
     * @return A unique pointer to a ConfigBase object representing the configuration.
     */
    static std::unique_ptr<ConfigBase> make_config_from_map(
        const std::unordered_map<std::string, std::string>& config_map);
};

/**
 * @brief Reader class for CSV files.
 *
 * This class extends ReaderBase and implements reading configuration data from CSV files.
 */
class ReaderCSV : public ReaderBase {
  public:
    /**
     * @brief Method to read the configuration from the CSV file.
     *
     * @param app The CLI app subcommand containing the options for the CSV input.
     * @return A unique pointer to a ConfigBase object representing the read configuration.
     */
    std::unique_ptr<ConfigBase> read(CLI::App* app, bool verbose) override;

  private:
    friend class ReaderCSVTester;  //!< For unit testing purposes.
    /**
     * @brief Helper method to split a CSV line into individual fields.
     *
     * @param line The input CSV line.
     * @return A vector of strings representing the individual fields.
     */
    std::vector<std::string> splitCsvLine(const std::string& line) const;
};

/**
 * @brief Reader class for DAT files.
 *
 * This class extends ReaderBase and implements reading configuration data from DAT files.
 */
class ReaderDAT : public ReaderBase {
  public:
    /**
     * @brief Method to read the configuration from the DAT file.
     *
     * @param app The CLI app subcommand containing the options for the DAT input.
     * @return A unique pointer to a ConfigBase object representing the read configuration.
     */
    std::unique_ptr<ConfigBase> read(CLI::App* app, bool verbose) override;
};

/**
 * @brief Reader class for CLI input.
 *
 * This class extends ReaderBase and implements reading configuration data from CLI input.
 */
class ReaderCLI : public ReaderBase {
  public:
    /**
     * @brief Read the configuration from CLI input.
     *
     * @param app The CLI app subcommand containing the options for the CLI input.
     * @return A unique pointer to a ConfigBase object representing the read configuration.
     */
    std::unique_ptr<ConfigBase> read(CLI::App* app, bool verbose) override;
};

#endif  // READER_HPP
