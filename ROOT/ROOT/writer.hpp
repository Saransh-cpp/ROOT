#ifndef ROOT_WRITER_HPP
#define ROOT_WRITER_HPP
#include <fstream>
#include <iostream>
#include <string>

#include "../../external/eigen/Eigen/Dense"

enum WritingMethod { CONSOLE, CSV, DAT, GNUPLOT };

/**
 * Polymorphic Writer and Printer classes for the results of the root finding process.
 * These classes themselves could be used for other kinds of writing, and do not have any relationship
 * with the solver process, except for being constructed there as object.
 * The Writer class handles the writing outer process, meanwhile the printers effectively write on the output or
 * in the given file the results.
 */

template <typename V>
class Printer;

/**
 * \brief Class which stores the values to write in an argument, constructs the Printer and handles the printing process
 */
template <typename T>
class Writer {
  protected:
    T values;
    WritingMethod method;

  public:
    Writer(const T& vals_to_write, WritingMethod write_method);
    /** \brief Method to run the printing loop and correctly initialize the Printer*/
    void writing_process(std::string filename = "");
    /** \brief Converts the generic Printer into a typed one for a specific output destination*/
    template <typename V>
    void choose_how(std::unique_ptr<Printer<V>>& printer, std::string filename);
    /** \brief Helper just to print the final result of the solver process*/
    void print_final_result() const;
};

/** The Printer classes actually print a given value in the specified output, getting just one value from the
 * ones stored in Writer object. They can write on the output, on .csv or .dat files, or they can
 * write on a .dat file and then create a gnuplot.
 */

/** \brief Abstract Printer class*/
template <typename V>
class Printer {
  public:
    Printer() = default;
    virtual ~Printer() = default;
    virtual void write_values(const V& value) = 0;
};

/** \brief The class to print out the values in the output*/
template <typename V>
class OutputPrinter : public Printer<V> {
  public:
    OutputPrinter();
    /** \brief Method to actually print a given value in the output*/
    void write_values(const V& value) override;
};

/** \brief Mother class for all the Printers which print in a file*/
template <typename V>
class FilePrinter : public Printer<V> {
  protected:
    /** \brief The name of output file, without the extension*/
    std::string filename;
    /** \brief Boolean which defines whether at a new Writing process the output file (if already existent), has to
     * be overwritten or not
     */
    bool append;
    /** \brief The actual file, saved in order not to access it at each printing iteration*/
    std::ofstream file;

  public:
    /** \brief The constructor gets as input the filename string and the boolean for overwriting option*/
    FilePrinter(const std::string& fname, bool ow_mode);
};

/** \brief Class to write on .dat the result, from which Gnu Printer will inherit*/
template <typename V>
class datPrinter : public FilePrinter<V> {
  public:
    /** @brief Constructor taking the filename and whether to overwrite or not the file */
    datPrinter(const std::string& fname, bool ow_mode);
    /** @brief Writes a given result into the .dat file*/
    void write_values(const V& value) override;
};

/** @brief Class to write on .csv the result*/
template <typename V>
class csvPrinter : public FilePrinter<V> {
  private:
    /** @brief Separator for the .csv output file, given at construction time */
    char separator;

  public:
    /** Constructor taking the filename, the separator and whether to overwrite or not the file */
    csvPrinter(const std::string& fname, char sep, bool ow_mode);
    /** @brief Writes a given result into the .csv file */
    void write_values(const V& value) override;
};

/** Daughter of datPrinter lass to write on .dat (inherited) and produce a gnu plot for the results*/
template <typename V>
// LLM
class GnuplotPrinter : public datPrinter<V> {
  public:
    /** @brief Constructor taking the filename and whether to overwrite the file or not */
    GnuplotPrinter(const std::string& fname, bool ow_mode);
    /** @brief Callsthe generation of the .png output plot */
    void generate_gnuplot_script() const;
};

#include "writer_impl.hpp"

#endif  // ROOT_WRITER_HPP
