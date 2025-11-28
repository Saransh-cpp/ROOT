#include <string>

#include "config.hpp"

class ReaderBase {
  public:
    std::string filename;
    virtual ConfigBase* read() = 0;
};

class ReaderCSV : public ReaderBase {
  public:
    ReaderCSV(const std::string& filename);
    ConfigBase* read() override;
};

class ReaderDAT : public ReaderBase {
  public:
    ReaderDAT(const std::string& filename);
    ConfigBase* read() override;
};

// class ReaderCLI : public ReaderBase {
//   public:
//     std::string method;

//     ReaderCLI();
//     ConfigBase* read() override;
// };
