#include "reader.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "config.hpp"
#include "function_parser.hpp"

ReaderBase::ReaderBase(const std::string& filename, const ReaderOptions& options)
    : filename(std::move(filename)), options(std::move(options)) {}
std::string ReaderBase::trim(const std::string& untrimmed_str) {
    size_t start = 0;
    while (start < untrimmed_str.size() && (std::isspace(static_cast<unsigned char>(untrimmed_str[start]))) != 0) {
        ++start;
    }
    size_t end = untrimmed_str.size();
    while (end > start && (std::isspace(static_cast<unsigned char>(untrimmed_str[end - 1]))) != 0) {
        --end;
    }
    return untrimmed_str.substr(start, end - start);
}

bool ReaderBase::parseBool(const std::string& bool_str, bool& out) {
    std::string bool_str_copy = bool_str;
    std::transform(bool_str_copy.begin(), bool_str_copy.end(), bool_str_copy.begin(),
                   [](unsigned char character) { return std::tolower(character); });
    if (bool_str_copy == "1" || bool_str_copy == "true" || bool_str_copy == "yes" || bool_str_copy == "y") {
        out = true;
        return true;
    }
    if (bool_str_copy == "0" || bool_str_copy == "false" || bool_str_copy == "no" || bool_str_copy == "n") {
        out = false;
        return true;
    }
    return false;
}

bool ReaderBase::parseDouble(const std::string& double_str, double& out) {
    try {
        size_t idx = 0;
        out = std::stod(double_str, &idx);
        return idx == double_str.size();
    } catch (...) {
        return false;
    }
}

bool ReaderBase::parseInt(const std::string& int_str, int& out) {
    try {
        size_t idx = 0;
        out = std::stoi(int_str, &idx);
        return idx == int_str.size();
    } catch (...) {
        return false;
    }
}

bool ReaderBase::parseMethod(const std::string& method_str, Method& out) {
    std::string method_str_copy = method_str;
    std::transform(method_str_copy.begin(), method_str_copy.end(), method_str_copy.begin(),
                   [](unsigned char character) { return std::tolower(character); });
    if (method_str_copy == "bisection" || method_str_copy == "bisect" || method_str_copy == "bisectionmethod") {
        out = Method::BISECTION;
        return true;
    }
    if (method_str_copy == "newton" || method_str_copy == "newtonmethod") {
        out = Method::NEWTON;
        return true;
    }
    if (method_str_copy == "secant" || method_str_copy == "secantmethod") {
        out = Method::SECANT;
        return true;
    }
    if (method_str_copy == "fixed_point" || method_str_copy == "fixedpoint" || method_str_copy == "fixed-point" ||
        method_str_copy == "iterative") {
        out = Method::FIXED_POINT;
        return true;
    }
    return false;
}

std::unique_ptr<ConfigBase> ReaderBase::make_config_from_map(
    const std::unordered_map<std::string, std::string>& config_map) {
    // method is required
    auto itm = config_map.find("method");
    if (itm == config_map.end()) {
        std::cerr << "make_config_from_map: required field 'method' missing\n";
        return nullptr;
    }
    Method method;  // NOLINT(cppcoreguidelines-init-variables)
    if (!parseMethod(itm->second, method)) {
        std::cerr << "make_config_from_map: unknown method: " << itm->second << "\n";
        return nullptr;
    }

    // shared optional params
    double tolerance = 1e-7;
    auto it_tol = config_map.find("tolerance");
    if (it_tol != config_map.end()) {
        if (!parseDouble(it_tol->second, tolerance)) {
            std::cerr << "make_config_from_map: invalid tolerance: " << it_tol->second << "\n";
            return nullptr;
        }
    }

    int max_iter = 100;
    auto it_max = config_map.find("max_iterations");
    if (it_max != config_map.end()) {
        if (!parseInt(it_max->second, max_iter)) {
            std::cerr << "make_config_from_map: invalid max_iterations: " << it_max->second << "\n";
            return nullptr;
        }
    }

    bool aitken = false;
    auto it_ait = config_map.find("aitken");
    if (it_ait != config_map.end()) {
        if (!parseBool(it_ait->second, aitken)) {
            std::cerr << "make_config_from_map: invalid aitken: " << it_ait->second << "\n";
            return nullptr;
        }
    }

    // functions
    auto it_fun = config_map.find("function");
    if (it_fun == config_map.end()) {
        std::cerr << "make_config_from_map: required field 'function' missing\n";
        return nullptr;
    }
    std::string function_str = it_fun->second;
    auto function = FunctionParserBase::parseFunction(function_str);

    // Build concrete config based on method
    switch (method) {
        case Method::BISECTION: {
            auto it_a = config_map.find("initial_point");
            auto it_b = config_map.find("final_point");
            if (it_a == config_map.end() || it_b == config_map.end()) {
                std::cerr << "make_config_from_map: bisection requires initial_point and final_point\n";
                return nullptr;
            }
            double initial_point = 0.0;
            double final_point = 0.0;
            if (!parseDouble(it_a->second, initial_point) || !parseDouble(it_b->second, final_point)) {
                std::cerr << "make_config_from_map: invalid bisection endpoints\n";
                return nullptr;
            }
            return std::make_unique<BisectionConfig>(tolerance, max_iter, aitken, function, initial_point, final_point);
        }

        case Method::NEWTON: {
            auto it_x0 = config_map.find("initial_guess");
            if (it_x0 == config_map.end()) {
                std::cerr << "make_config_from_map: newton requires initial_guess\n";
                return nullptr;
            }
            double initial_guess = 0.0;
            if (!parseDouble(it_x0->second, initial_guess)) {
                std::cerr << "make_config_from_map: invalid initial_guess\n";
                return nullptr;
            }
            auto it_df = config_map.find("derivative");
            if (it_df == config_map.end()) {
                std::cerr << "make_config_from_map: newton requires derivative function\n";
                return nullptr;
            }
            std::function<double(double)> function_derivative = FunctionParserBase::parseFunction(it_df->second);
            return std::make_unique<NewtonConfig>(tolerance, max_iter, aitken, function, function_derivative,
                                                  initial_guess);
        }

        case Method::SECANT: {
            auto it_x0 = config_map.find("initial_point");
            auto it_x1 = config_map.find("final_point");
            if (it_x0 == config_map.end() || it_x1 == config_map.end()) {
                std::cerr << "make_config_from_map: secant requires initial_point and final_point\n";
                return nullptr;
            }
            double initial_point = 0.0;
            double final_point = 0.0;
            if (!parseDouble(it_x0->second, initial_point) || !parseDouble(it_x1->second, final_point)) {
                std::cerr << "make_config_from_map: invalid secant initial points\n";
                return nullptr;
            }
            return std::make_unique<SecantConfig>(tolerance, max_iter, aitken, function, initial_point, final_point);
        }

        case Method::FIXED_POINT: {
            auto it_x0 = config_map.find("initial_guess");
            auto it_g = config_map.find("function_g");
            if (it_x0 == config_map.end() || it_g == config_map.end()) {
                std::cerr << "make_config_from_map: fixed_point requires initial_guess and function_g\n";
                return nullptr;
            }
            double initial_guess = 0.0;
            auto function_g = FunctionParserBase::parseFunction(it_g->second);
            return std::make_unique<FixedPointConfig>(tolerance, max_iter, aitken, function, initial_guess, function_g);
        }
    }  // switch

    return nullptr;  // unreachable
}

ReaderCSV::ReaderCSV(const std::string& filename, const ReaderOptions& options) : ReaderBase(filename, options) {}

std::vector<std::string> ReaderCSV::splitCsvLine(const std::string& line) const {
    std::vector<std::string> fields;
    std::string cur;
    bool in_quote = false;
    char sep = options.sep;
    char quote = options.quote;
    for (size_t i = 0; i < line.size(); ++i) {
        char character = line[i];
        if (in_quote) {
            if (character == quote) {
                if (i + 1 < line.size() && line[i + 1] == quote) {
                    cur.push_back(quote);
                    ++i;
                } else {
                    in_quote = false;
                }
            } else {
                cur.push_back(character);
            }
        } else {
            if (character == quote) {
                in_quote = true;
            } else if (character == sep) {
                fields.push_back(cur);
                cur.clear();
            } else {
                cur.push_back(character);
            }
        }
    }
    fields.push_back(cur);
    return fields;
}

std::unique_ptr<ConfigBase> ReaderCSV::read() {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "ReaderCSV: failed to open file: " << filename << "\n";
        return nullptr;
    }

    std::string headerLine;
    std::string valueLine;

    if (options.has_header) {
        if (!std::getline(ifs, headerLine)) {
            std::cerr << "ReaderCSV: empty file (expecting header)\n";
            return nullptr;
        }
        if (!std::getline(ifs, valueLine)) {
            std::cerr << "ReaderCSV: missing value row\n";
            return nullptr;
        }
    } else {
        if (!std::getline(ifs, valueLine)) {
            std::cerr << "ReaderCSV: empty file\n";
            return nullptr;
        }
        headerLine.clear();
    }

    std::vector<std::string> headers;
    if (options.has_header) {
        headers = splitCsvLine(headerLine);
        for (auto& header : headers) {
            header = trim(header);
        }
    } else if (!options.headers.empty()) {
        headers = options.headers;  // user supplied header names
    }

    auto values = splitCsvLine(valueLine);
    for (auto& value : values) {
        value = trim(value);
    }

    std::unordered_map<std::string, std::string> config_map;
    if (!headers.empty()) {
        if (headers.size() != values.size()) {
            std::cerr << "ReaderCSV: header/value columns mismatch\n";
            return nullptr;
        }
        for (size_t i = 0; i < headers.size(); ++i) {
            std::string key = headers[i];
            std::transform(key.begin(), key.end(), key.begin(),
                           [](unsigned char character) { return std::tolower(character); });
            config_map[key] = values[i];
        }
    } else {
        // positional mapping documented here:
        std::vector<std::string> posnames = {"method",     "tolerance",    "max_iterations", "aitken",
                                             "function",   "derivative",   "initial_point",  "final_point",
                                             "function_g", "initial_guess"};
        for (size_t i = 0; i < values.size() && i < posnames.size(); ++i) {
            config_map[posnames[i]] = values[i];
        }
    }

    return make_config_from_map(config_map);
}

ReaderDAT::ReaderDAT(const std::string& filename, const ReaderOptions& options) : ReaderBase(filename, options) {}

std::unique_ptr<ConfigBase> ReaderDAT::read() {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "ReaderDAT: failed to open file: " << filename << "\n";
        return nullptr;
    }

    std::unordered_map<std::string, std::string> config_map;
    std::string line;
    size_t lineno = 0;
    while (std::getline(ifs, line)) {
        ++lineno;
        line = trim(line);
        if (line.empty()) {
            continue;
        }
        if (line[0] == '#') {
            continue;
        }

        auto equals = line.find('=');
        if (equals == std::string::npos) {
            std::cerr << "ReaderDAT: malformed line " << lineno << " (no '=')\n";
            return nullptr;
        }
        std::string key = trim(line.substr(0, equals));
        std::string val = trim(line.substr(equals + 1));
        std::transform(key.begin(), key.end(), key.begin(),
                       [](unsigned char character) { return std::tolower(character); });
        config_map[key] = val;
    }

    return make_config_from_map(config_map);
}
