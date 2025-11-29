#ifndef ROOT_WRITER_HPP
#define ROOT_WRITER_HPP
#include <fstream>
#include <iostream>
#include <string>

#include "../external/eigen/Eigen/Dense"

class Printer;

class Writer {
  protected:
    Eigen::MatrixX2d values;

  public:
    Writer(const Eigen::MatrixX2d& vals_to_write);
    void writing_process();
    void choose_how(std::unique_ptr<Printer>& printer);
    void print_final_result() const;
};

class Printer {
  public:
    Printer() = default;
    virtual ~Printer() = default;
    virtual void write_values(const Eigen::Vector2d& value) = 0;
};

class OutputPrinter : public Printer {
  public:
    OutputPrinter();
    void write_values(const Eigen::Vector2d& value) override;
};

class FilePrinter : public Printer {
  protected:
    std::string filename;

  public:
    FilePrinter();
};

class datPrinter : public FilePrinter {
  public:
    datPrinter();
    void write_values(const Eigen::Vector2d& value) override;
};

class csvPrinter : public FilePrinter {
  private:
    char separator;

  public:
    csvPrinter();
    void write_values(const Eigen::Vector2d& value) override;
};

// LLM
class GnuplotPrinter : public Printer {
  private:
    std::string filename;

  public:
    GnuplotPrinter(const std::string& fname) : filename(fname + ".dat") {}

    void write_values(const Eigen::Vector2d& value) override {
        std::ofstream file(filename, std::ios::app);
        file << value(0) << " " << value(1) << "\n";
    }

    void generate_gnuplot_script() const {
        std::ofstream script("plot.plt");
        script << "set terminal pngcairo size 800,600\n"
               << "set output 'plot.png'\n"
               << "plot '" << filename << "' using 1:2 with linespoints\n";
    }
};

#endif  // ROOT_WRITER_HPP
