#include <string>

#include "config.hpp"

class ReaderBase {
  public:
    virtual ConfigBase* read(std::string filename) = 0;
};

class ReaderCSV : public ReaderBase {
  public:
    ConfigBase* read(std::string filename) override;
};

class ReaderDAT : public ReaderBase {
  public:
    ConfigBase* read(std::string filename) override;
};
