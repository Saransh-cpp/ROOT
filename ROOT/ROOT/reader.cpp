#include "reader.hpp"

#include <string>

#include "config.hpp"

ConfigBase* ReaderCSV ::read(std::string filename) {
    ConfigBase* config = nullptr;
    config = new ConfigBase();
    return config;
};

ConfigBase* ReaderDAT ::read(std::string filename) {
    ConfigBase* config = nullptr;
    config = new ConfigBase();
    return config;
};
