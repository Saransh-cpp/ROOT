#include "reader.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "config.hpp"
#include "function_parser.hpp"

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
    if (method_str_copy == "chords" || method_str_copy == "chordsmethod") {
        out = Method::CHORDS;
        return true;
    }
    if (method_str_copy == "fixed_point" || method_str_copy == "fixedpoint" || method_str_copy == "fixed-point" ||
        method_str_copy == "fixed_point") {
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
        std::cerr << "\033[31mmake_config_from_map: required field 'method' missing\033[0m\n";
        std::exit(EXIT_FAILURE);
    }
    Method method;  // NOLINT(cppcoreguidelines-init-variables)
    if (!parseMethod(itm->second, method)) {
        std::cerr << "\033[31mmake_config_from_map: unknown method: " << itm->second << "\033[0m\n";
        std::exit(EXIT_FAILURE);
    }

    // shared optional params
    double tolerance = 1e-7;
    auto it_tol = config_map.find("tolerance");
    if (it_tol != config_map.end()) {
        if (!parseDouble(it_tol->second, tolerance)) {
            std::cerr << "\033[31mmake_config_from_map: invalid tolerance: " << it_tol->second << "\033[0m\n";
            std::exit(EXIT_FAILURE);
        }
    }

    int max_iter = 100;
    auto it_max = config_map.find("max-iterations");
    if (it_max != config_map.end()) {
        if (!parseInt(it_max->second, max_iter)) {
            std::cerr << "\033[31mmake_config_from_map: invalid max-iterations: " << it_max->second << "\033[0m\n";
            std::exit(EXIT_FAILURE);
        }
    }

    bool aitken = false;
    auto it_ait = config_map.find("aitken");
    if (it_ait != config_map.end()) {
        if (!parseBool(it_ait->second, aitken)) {
            std::cerr << "\033[31mmake_config_from_map: invalid aitken: " << it_ait->second << "\033[0m\n";
            std::exit(EXIT_FAILURE);
        }
    }

    // functions
    auto it_fun = config_map.find("function");
    if (it_fun == config_map.end()) {
        std::cerr << "\033[31mmake_config_from_map: required field 'function' missing\033[0m\n";
        std::exit(EXIT_FAILURE);
    }
    std::string function_str = it_fun->second;
    auto function = FunctionParserBase::parseFunction(function_str);

    // verbose
    auto it_verb = config_map.find("verbose");
    bool verbose = false;
    if (it_verb != config_map.end()) {
        if (!parseBool(it_verb->second, verbose)) {
            std::cerr << "\033[31mmake_config_from_map: invalid verbose: " << it_verb->second << "\033[0m\n";
            std::exit(EXIT_FAILURE);
        }
    }

    // Build concrete config based on method
    switch (method) {
        case Method::BISECTION: {
            auto it_a = config_map.find("interval_a");
            auto it_b = config_map.find("interval_b");
            if (it_a == config_map.end() || it_b == config_map.end()) {
                std::cerr << "\033[31mmake_config_from_map: bisection requires interval_a and interval_b\033[0m\n";
                std::exit(EXIT_FAILURE);
            }
            double interval_a = 0.0;
            double interval_b = 0.0;
            if (!parseDouble(it_a->second, interval_a) || !parseDouble(it_b->second, interval_b)) {
                std::cerr << "\033[31mmake_config_from_map: invalid bisection endpoints\033[0m\n";
                std::exit(EXIT_FAILURE);
            }
            return std::make_unique<BisectionConfig>(tolerance, max_iter, aitken, function, interval_a, interval_b,
                                                     verbose);
        }

        case Method::NEWTON: {
            auto it_x0 = config_map.find("initial");
            if (it_x0 == config_map.end()) {
                std::cerr << "\033[31mmake_config_from_map: newton requires initial\033[0m\n";
                std::exit(EXIT_FAILURE);
            }
            double initial = 0.0;
            if (!parseDouble(it_x0->second, initial)) {
                std::cerr << "\033[31mmake_config_from_map: invalid initial\033[0m\n";
                std::exit(EXIT_FAILURE);
            }
            auto it_df = config_map.find("derivative");
            if (it_df == config_map.end()) {
                std::cerr << "\033[31mmake_config_from_map: newton requires derivative function\033[0m\n";
                std::exit(EXIT_FAILURE);
            }
            std::function<double(double)> function_derivative = FunctionParserBase::parseFunction(it_df->second);
            return std::make_unique<NewtonConfig>(tolerance, max_iter, aitken, function, function_derivative, initial,
                                                  verbose);
        }

        case Method::CHORDS: {
            auto it_x0 = config_map.find("x0");
            auto it_x1 = config_map.find("x1");
            if (it_x0 == config_map.end() || it_x1 == config_map.end()) {
                std::cerr << "\033[31mmake_config_from_map: chords requires two initial points\033[0m\n";
                std::exit(EXIT_FAILURE);
            }
            double initial_point1 = 0.0;
            double initial_point2 = 0.0;
            if (!parseDouble(it_x0->second, initial_point1) || !parseDouble(it_x1->second, initial_point2)) {
                std::cerr << "\033[31mmake_config_from_map: invalid chords initial points\033[0m\n";
                std::exit(EXIT_FAILURE);
            }
            return std::make_unique<ChordsConfig>(tolerance, max_iter, aitken, function, initial_point1, initial_point2,
                                                  verbose);
        }

        case Method::FIXED_POINT: {
            auto it_x0 = config_map.find("initial");
            auto it_g = config_map.find("g-function");
            if (it_x0 == config_map.end() || it_g == config_map.end()) {
                std::cerr << "\033[31mmake_config_from_map: fixed_point requires initial and g-function\033[0m\n";
                std::exit(EXIT_FAILURE);
            }
            double initial = 0.0;
            if (!parseDouble(it_x0->second, initial)) {
                std::cerr << "\033[31mmake_config_from_map: invalid initial\033[0m\n";
                std::exit(EXIT_FAILURE);
            }
            auto g_function = FunctionParserBase::parseFunction(it_g->second);
            return std::make_unique<FixedPointConfig>(tolerance, max_iter, aitken, function, initial, g_function,
                                                      verbose);
        }
    }  // switch

    return nullptr;  // unreachable
}

std::vector<std::string> ReaderCSV::splitCsvLine(const std::string& line) const {
    std::vector<std::string> fields;
    std::string cur;
    bool in_quote = false;
    char sep = this->sep;
    char quote = this->quote;
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

std::unique_ptr<ConfigBase> ReaderCSV::read(CLI::App* app, bool verbose) {
    this->filename = app->get_option("--file")->as<std::string>();
    this->sep = app->get_option("--sep")->as<char>();
    this->quote = app->get_option("--quote")->as<char>();
    this->has_header = app->get_option("--header")->as<bool>();
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "\033[31mReaderCSV: failed to open file: " << filename << "\033[0m\n";
        std::exit(EXIT_FAILURE);
    }

    std::string headerLine;
    std::string valueLine;

    if (this->has_header) {
        if (!std::getline(ifs, headerLine)) {
            std::cerr << "\033[31mReaderCSV: empty file (expecting header)\033[0m\n";
            std::exit(EXIT_FAILURE);
        }
        if (!std::getline(ifs, valueLine)) {
            std::cerr << "\033[31mReaderCSV: missing value row\033[0m\n";
            std::exit(EXIT_FAILURE);
        }
    } else {
        if (!std::getline(ifs, valueLine)) {
            std::cerr << "\033[31mReaderCSV: empty file\033[0m\n";
            std::exit(EXIT_FAILURE);
        }
        headerLine.clear();
    }

    std::vector<std::string> headers;
    if (this->has_header) {
        headers = splitCsvLine(headerLine);
        for (auto& header : headers) {
            header = trim(header);
        }
    } else {
        std::cerr << "\033[31mReaderCSV: no headers provided\033[0m\n";
        std::exit(EXIT_FAILURE);
    }

    auto values = splitCsvLine(valueLine);
    for (auto& value : values) {
        value = trim(value);
    }

    std::unordered_map<std::string, std::string> config_map;
    if (!headers.empty()) {
        if (headers.size() != values.size()) {
            std::cerr << "\033[31mReaderCSV: header/value columns mismatch\033[0m\n";
            std::exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < headers.size(); ++i) {
            std::string key = headers[i];
            std::transform(key.begin(), key.end(), key.begin(),
                           [](unsigned char character) { return std::tolower(character); });
            config_map[key] = values[i];
        }
    } else {
        // positional mapping documented here:
        std::vector<std::string> posnames = {"method",     "tolerance",  "max-iterations", "aitken",     "function",
                                             "derivative", "interval_a", "interval_b",     "function-g", "initial",
                                             "x0",         "x1"};
        for (size_t i = 0; i < values.size() && i < posnames.size(); ++i) {
            config_map[posnames[i]] = values[i];
        }
    }

    if (verbose) {
        config_map["verbose"] = "true";
        std::cout << "ReaderCSV: read configuration:\n";
        for (const auto& item : config_map) {
            std::cout << "  " << item.first << " = " << item.second << "\n";
        }
    } else {
        config_map["verbose"] = "false";
    }

    return make_config_from_map(config_map);
}

std::unique_ptr<ConfigBase> ReaderDAT::read(CLI::App* app, bool verbose) {
    this->filename = app->get_option("--file")->as<std::string>();
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "\033[31mReaderDAT: failed to open file: " << filename << "\033[0m\n";
        std::exit(EXIT_FAILURE);
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
            std::cerr << "\033[31mReaderDAT: malformed line " << lineno << " (no '=')\033[0m\n";
            std::exit(EXIT_FAILURE);
        }
        std::string key = trim(line.substr(0, equals));
        std::string val = trim(line.substr(equals + 1));
        std::transform(key.begin(), key.end(), key.begin(),
                       [](unsigned char character) { return std::tolower(character); });
        config_map[key] = val;
    }

    if (verbose) {
        config_map["verbose"] = "true";
        std::cout << "ReaderDAT: read configuration:\n";
        for (const auto& item : config_map) {
            std::cout << "  " << item.first << " = " << item.second << "\n";
        }
    } else {
        config_map["verbose"] = "false";
    }

    return make_config_from_map(config_map);
}

std::unique_ptr<ConfigBase> ReaderCLI::read(CLI::App* app, bool verbose) {
    if (verbose) {
        std::cout << "ReaderCLI: read configuration\n";
        std::cout << "  tolerance = " << app->get_option("--tolerance")->as<double>() << "\n";
        std::cout << "  max-iterations = " << app->get_option("--max-iterations")->as<int>() << "\n";
        std::cout << "  aitken = " << (app->get_option("--aitken")->as<bool>() ? "true" : "false") << "\n";
        std::cout << "  function = " << app->get_option("--function")->as<std::string>() << "\n";
        std::cout << "  verbose = " << (verbose ? "true" : "false") << "\n";
    }
    std::unique_ptr<ConfigBase> config;
    if (*app->get_subcommand("newton")) {
        config = std::make_unique<NewtonConfig>(
            app->get_option("--tolerance")->as<double>(), app->get_option("--max-iterations")->as<int>(),
            app->get_option("--aitken")->as<bool>(),
            FunctionParserBase::parseFunction(app->get_option("--function")->as<std::string>()),
            FunctionParserBase::parseFunction(
                app->get_subcommand("newton")->get_option("--derivative")->as<std::string>()),
            app->get_subcommand("newton")->get_option("--initial")->as<double>(), verbose);
        if (verbose) {
            std::cout << "  derivative = "
                      << app->get_subcommand("newton")->get_option("--derivative")->as<std::string>() << "\n";
            std::cout << "  initial = " << app->get_subcommand("newton")->get_option("--initial")->as<double>() << "\n";
        }
    } else if (*app->get_subcommand("chords")) {
        config = std::make_unique<ChordsConfig>(
            app->get_option("--tolerance")->as<double>(), app->get_option("--max-iterations")->as<int>(),
            app->get_option("--aitken")->as<bool>(),
            FunctionParserBase::parseFunction(app->get_option("--function")->as<std::string>()),
            app->get_subcommand("chords")->get_option("--x0")->as<double>(),
            app->get_subcommand("chords")->get_option("--x1")->as<double>(), verbose);
        if (verbose) {
            std::cout << "  x0 = " << app->get_subcommand("chords")->get_option("--x0")->as<double>() << "\n";
            std::cout << "  x1 = " << app->get_subcommand("chords")->get_option("--x1")->as<double>() << "\n";
        }
    } else if (*app->get_subcommand("fixed_point")) {
        config = std::make_unique<FixedPointConfig>(
            app->get_option("--tolerance")->as<double>(), app->get_option("--max-iterations")->as<int>(),
            app->get_option("--aitken")->as<bool>(),
            FunctionParserBase::parseFunction(app->get_option("--function")->as<std::string>()),
            app->get_subcommand("fixed_point")->get_option("--initial")->as<double>(),
            FunctionParserBase::parseFunction(
                app->get_subcommand("fixed_point")->get_option("--g-function")->as<std::string>()),
            verbose);
        if (verbose) {
            std::cout << " g-function = "
                      << app->get_subcommand("fixed_point")->get_option("--g-function")->as<std::string>() << "\n";
            std::cout << "  initial = " << app->get_subcommand("fixed_point")->get_option("--initial")->as<double>()
                      << "\n";
        }
    } else if (*app->get_subcommand("bisection")) {
        config = std::make_unique<BisectionConfig>(
            app->get_option("--tolerance")->as<double>(), app->get_option("--max-iterations")->as<int>(),
            app->get_option("--aitken")->as<bool>(),
            FunctionParserBase::parseFunction(app->get_option("--function")->as<std::string>()),
            app->get_subcommand("bisection")->get_option("--interval_a")->as<double>(),
            app->get_subcommand("bisection")->get_option("--interval_b")->as<double>(), verbose);
        if (verbose) {
            std::cout << "  interval_a = " << app->get_subcommand("bisection")->get_option("--interval_a")->as<double>()
                      << "\n";
            std::cout << "  interval_b = " << app->get_subcommand("bisection")->get_option("--interval_b")->as<double>()
                      << "\n";
        }
    }
    return config;
}
