#ifndef ROOT_WRITER_HPP
#define ROOT_WRITER_HPP
#include <string>

#include "Eigen/Dense"

class Printer;

class Writer {
  protected:
    Eigen::Matrix2d values;

  public:
    Writer(Eigen::Matrix2d& vals_to_write);
    void writing_process();
    void choose_how(auto printer);
    void print_final_result();
};

class Printer {
  public:
    Printer(Writer& writer);
    virtual void write_values(Eigen::Vector2d value) = 0;
};

class VisualPrinter : public Printer {
  public:
    VisualPrinter(Writer& writer);
};

class OutputPrinter : public Printer {
  public:
};

class FilePrinter : public Printer {
  private:
    std::string filename;

  public:
};

class datPrinter : public FilePrinter {
  public:
};

class csvPrinter : public FilePrinter {
  private:
    char separator;

  public:
};

#endif  // ROOT_WRITER_HPP
