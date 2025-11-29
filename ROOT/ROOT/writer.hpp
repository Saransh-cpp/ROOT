#ifndef ROOT_WRITER_HPP
#define ROOT_WRITER_HPP
#include <string>

#include "../external/eigen/Eigen/Dense"

class Printer;

class Writer {
  protected:
    Eigen::Matrix2d values;

  public:
    Writer(Eigen::Matrix2d& vals_to_write);
    void writing_process();
    void choose_how(std::unique_ptr<Printer>& printer);
    void print_final_result();
};

class Printer {
  public:
    Printer() = default;
    virtual void write_values(Eigen::Vector2d value) = 0;
};

class VisualPrinter : public Printer {
  private:
    std::string filename;

  public:
    VisualPrinter();
    void write_values(Eigen::Vector2d value) override;
};

class OutputPrinter : public Printer {
  public:
    OutputPrinter();
    void write_values(Eigen::Vector2d value) override;
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
    void write_values(Eigen::Vector2d value) override;
};

class csvPrinter : public FilePrinter {
  private:
    char separator;

  public:
    csvPrinter();
    void write_values(Eigen::Vector2d value) override;
};

#endif  // ROOT_WRITER_HPP
