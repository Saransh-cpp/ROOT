#ifndef ROOT_WRITER_HPP
#define ROOT_WRITER_HPP
#include <string>

#include "Eigen/Dense"

class Writer {
  protected:
    Eigen::Matrix2d values;

  public:
    Writer(Eigen::Matrix2d& vals_to_write);
    void writing_process();
    void choose_how();
    virtual void write_values() = 0;
};

class VisualWriter : public Writer {
  public:
    void write_values() override;
};

class OutputWriter : public Writer {
  public:
    void write_values() override;
};

class FileWriter : public Writer {
  private:
    std::string filename;

  public:
    void write_values() override = 0;
};

class datWriter : public FileWriter {
  public:
    void write_values() override;
};

class csvWriter : public FileWriter {
  private:
    char separator;

  public:
    void write_values() override;
};

#endif  // ROOT_WRITER_HPP
