#include "reader.hpp"

#include <string>

#include "config.hpp"

ReaderCSV ::ReaderCSV(const std::string& filename) { this->filename = filename; }
ConfigBase* ReaderCSV ::read() {
    ConfigBase* config = nullptr;
    config = new ConfigBase();
    return config;
};

ReaderDAT ::ReaderDAT(const std::string& filename) { this->filename = filename; }
ConfigBase* ReaderDAT ::read() {
    ConfigBase* config = nullptr;
    config = new ConfigBase();
    return config;
};
