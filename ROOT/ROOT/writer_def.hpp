/**
 * @file writer_def.hpp
 * @brief Contains definitions for classes Writer and Printer to Write on different output destinations
 *
 * @author andreasaporito
 *
 * These classes themselves could be used for other kinds of writing, and do not have any relationship
 * with the solver process.
 * The Writer class handles the writing outer process, meanwhile the printers effectively write on the output or
 * in the given file the results.
 * The Printer classes actually print a given value in the specified output, getting just one value from the
 * ones stored in Writer object. They can write on the output, on .csv or .dat files, or they can
 * write on a .dat file and then create a gnuplot.
 */
#ifndef ROOT_WRITER_DEF_HPP
#define ROOT_WRITER_DEF_HPP
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <string>

enum WritingMethod { CONSOLE, CSV, DAT, GNUPLOT };

template <typename V>
class PrinterBase;

/**
 * @brief Writer class - stores required arguments and handles the printing flow
 */
template <typename T>
class Writer {
  private:
    friend class WriterBaseTester;

  protected:
    T values;              //!< Templated values to write to allow different usage of the class
    WritingMethod method;  //!< Method to write with - defined thanks to @author Saransh-cpp config
    char separator;        //!< Separator for .csv files
    bool overwrite;        //!< Option to overwrite or append the output file
    std::string filename;  //!< Name of the output file

  public:
    /**
     * @brief Constructor for a Writer object
     *
     * @param vals_to_write Values to be written, which will be stored in values argument
     * @param write_method Method to be used, which will be stored in method argument
     * @param separator Separator for .csv extension files (optional)
     * @param overwrite Option to overwrite or append to the file (optional)
     */
    Writer(const T& vals_to_write, WritingMethod write_method, std::string filename = "output", char separator = ',',
           bool overwrite = true);
    /** @brief Method to run the printing loop and correctly initialize the Printer
     *
     */
    void write();
    /** @brief Method to convert the generic Printer into a typed one for a specific output destination
     *
     * @param printer The original abstract printer
     */
    template <typename V>
    void build_printer(std::unique_ptr<PrinterBase<V>>& printer);
    /** @brief Helper just to print the final result of the solver process*/
    void print_final_result() const;
};

/** @brief Abstract Printer class*/
template <typename V>
class PrinterBase {
  public:
    PrinterBase() = default;
    virtual ~PrinterBase() = default;
    virtual void write_values(const V& value) = 0;
};

/** @brief The class to print out the values in the CLI*/
template <typename V>
class PrinterCLI : public PrinterBase<V> {
  public:
    /** @brief Constructor for the PrinterCLI class - just prints out a string*/
    PrinterCLI();
    /** @brief Method to actually print a given value in the output
     *
     * @param value The given value to be printed
     */
    void write_values(const V& value) override;
};

/** @brief Mother class for all the Printers which print in a file*/
template <typename V>
class PrinterFile : public PrinterBase<V> {
  protected:
    std::string filename;  //!< The name of output file, without the extension
    bool append;  //!< Boolean which defines whether at a new Writing process the output file (if already existent), has
                  //!< to be overwritten or not
    std::ofstream file;  //!< The actual file, saved in order not to access it at each printing iteration

  public:
    /** @brief The constructor for PrinterFile class
     *
     * @param fname Name of the file to write on
     * @param ow_mode Option to overwrite or append to the file
     */
    PrinterFile(const std::string& fname, bool ow_mode);
};

/** @brief Class to write on .dat the result - daughter of FilePrinter and Mother of GnuPlotPrinter*/
template <typename V>
class PrinterDAT : public PrinterFile<V> {
  public:
    /** @brief Constructor for the PrinterDAT class
     *
     * @param fname The name of the file to write on
     * @param ow_mode Option to overwrite or append the file
     */
    PrinterDAT(const std::string& fname, bool ow_mode);
    /** @brief Writes a given result into the .dat file
     *
     * @param value The value to write
     */
    void write_values(const V& value) override;
};

/** @brief Class to write on .csv the result*/
template <typename V>
class PrinterCSV : public PrinterFile<V> {
  private:
    char separator;  //!< Separator for the .csv output file, given at construction time

  public:
    /** @brief Constructor of the PrinterCSV class
     *
     * @param fname The file to write on
     * @param sep The separator to use
     * @param ow_mode Option to overwrite or append the file
     */
    PrinterCSV(const std::string& fname, char sep, bool ow_mode);
    /** @brief Writes a given result into the .csv file with the sotred separator */
    void write_values(const V& value) override;
};

/** @brief Class Daughter of PrinterDAT to write on .dat (inherited) and produce a gnu plot for the results*/
template <typename V>
// LLM
class PrinterGNUPlot : public PrinterDAT<V> {
  public:
    /** @brief Constructor for the PrinterGNUPlot
     *
     * @param fname The file to write on
     * @param ow_mode Option to overwrite or append the file
     */
    PrinterGNUPlot(const std::string& fname, bool ow_mode);
    /** @brief Calls the generation of the .png output plot */
    void generate_gnuplot_script() const;
};

#include "writer.hpp"

#endif  // ROOT_WRITER_HPP
