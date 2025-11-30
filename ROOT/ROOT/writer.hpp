#ifndef ROOT_WRITER_HPP
#define ROOT_WRITER_HPP
#include <fstream>
#include <iostream>
#include <string>

#include "../external/eigen/Eigen/Dense"
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
 * \brief Class which stores the values to write in an argument
 */
template <typename T>
class Writer {
  protected:
    T values;

  public:
    Writer(const T& vals_to_write);
    void writing_process();
    template <typename V>
    void choose_how(std::unique_ptr<Printer<V>>& printer);
    void print_final_result() const;
};

template <typename V>
class Printer {
  public:
    Printer() = default;
    virtual ~Printer() = default;
    virtual void write_values(const V& value) = 0;
};

template <typename V>
class OutputPrinter : public Printer<V> {
  public:
    OutputPrinter();
    void write_values(const V& value) override;
};

template <typename V>
class FilePrinter : public Printer<V> {
  protected:
    std::string filename;
    bool append;
    std::ofstream file;

  public:
    FilePrinter(const std::string& fname, bool ow_mode);
};

template <typename V>
class datPrinter : public FilePrinter<V> {
  public:
    datPrinter(const std::string& fname, bool ow_mode);
    void write_values(const V& value) override;
};

template <typename V>
class csvPrinter : public FilePrinter<V> {
  private:
    char separator;

  public:
    csvPrinter(const std::string& fname, char sep, bool ow_mode);
    void write_values(const V& value) override;
};

template <typename V>
// LLM
class GnuplotPrinter : public FilePrinter<V> {
  public:
    GnuplotPrinter(const std::string& fname, bool ow_mode = true) : FilePrinter<V>(fname + ".dat", ow_mode) {}

    void write_values(const V& value) override;

    void generate_gnuplot_script() const;
};

#endif  // ROOT_WRITER_HPP
