#include <CLI/CLI.hpp>
#include <functional>
#include <iostream>
#include <string>

#include "config.hpp"
#include "function_parser.hpp"
#include "reader.hpp"

int main(int argc, char** argv) {
    // ------------------------------------------------------------
    // Command-line interface
    // ------------------------------------------------------------
    CLI::App app{"ROOT Command Line Interface"};
    app.require_subcommand(1);

    // Global options shared across methods
    bool verbose = false;
    app.add_flag("-v,--verbose", verbose, "Enable verbose output (applies to all methods)");

    // Subcommands for each input type
    // Subcommand for CSV input
    auto* csv = app.add_subcommand("csv", "Use CSV input");
    std::string csv_file;
    csv->add_option("--file", csv_file, "Path to CSV file containing input data")->required();

    // Subcommand for dat input
    auto* dat = app.add_subcommand("dat", "Use DAT input");
    std::string dat_file;
    dat->add_option("--file", dat_file, "Path to DAT file containing input data")->required();

    // Subcommand for CLI input
    auto* cli = app.add_subcommand("cli", "Use CLI input");

    bool aitken = false;
    cli->add_flag("-a,--aitken", aitken, "Enable Aitken acceleration");

    double tolerance = 1e-7;
    cli->add_option("-t,--tolerance", tolerance, "Tolerance for convergence")->check(CLI::PositiveNumber);

    int max_iterations = 100;
    cli->add_option("-n,--max-iterations", max_iterations, "Maximum number of iterations")->check(CLI::PositiveNumber);

    std::string function_str;
    cli->add_option("-f,--function", function_str,
                    "Function to find root of (only polynomial and simple trig expressions)")
        ->required();
    cli->require_subcommand(1);

    // Subcommands for CLI input
    // newton
    auto* newton = cli->add_subcommand("newton", "Use Newton's method");
    double newton_initial = 0.0;
    std::string derivative_function;
    newton->add_option("--initial", newton_initial, "Initial guess x0 for Newton's method")->required();
    newton->add_option("--derivative", derivative_function, "Derivative of the function (optional)")->required();

    // secant
    auto* secant = cli->add_subcommand("secant", "Use Secant method");
    double secant_x0 = 0.0;
    double secant_x1 = 1.0;
    secant->add_option("--x0", secant_x0, "First initial guess x0")->required();
    secant->add_option("--x1", secant_x1, "Second initial guess x1")->required();

    // iterative (fixed-point)
    auto* iterative = cli->add_subcommand("iterative", "Use fixed-point iterative method");
    double iterative_initial = 0.0;
    std::string function_g;
    iterative->add_option("--initial", iterative_initial, "Initial guess x0 for iterative method")->required();
    iterative->add_option("--g-function", function_g, "g(x) for fixed-point iteration")->required();

    // bisection
    auto* bisection = cli->add_subcommand("bisection", "Use Bisection method");
    double interval_a = 0.0;
    double interval_b = 1.0;
    bisection->add_option("--interval_a", interval_a, "Left endpoint a")->required();
    bisection->add_option("--interval_b", interval_b, "Right endpoint b")->required();

    CLI11_PARSE(app, argc, argv);

    // ------------------------------------------------------------
    // Solver configuration
    // ------------------------------------------------------------
    ConfigBase* config = nullptr;

    // If CSV subcommand used, delegate to CSV reader
    if (*csv) {
        ReaderCSV csvReader(csv_file);
        config = csvReader.read();
    } else if (*dat) {
        ReaderDAT datReader(dat_file);
        config = datReader.read();
    } else if (*cli) {
        if (*newton) {
            config =
                new NewtonConfig(tolerance, max_iterations, aitken, FunctionParserBase::parseFunction(function_str),
                                 FunctionParserBase::parseFunction(derivative_function), newton_initial);
        } else if (*secant) {
            config = new SecantConfig(tolerance, max_iterations, aitken,
                                      FunctionParserBase::parseFunction(function_str), secant_x0, secant_x1);
        } else if (*iterative) {
            config =
                new FixedPointConfig(tolerance, max_iterations, aitken, FunctionParserBase::parseFunction(function_str),
                                     iterative_initial, FunctionParserBase::parseFunction(function_g));
        } else if (*bisection) {
            config = new BisectionConfig(tolerance, max_iterations, aitken,
                                         FunctionParserBase::parseFunction(function_str), interval_a, interval_b);
        } else {
            std::cerr << "No valid subcommand provided." << std::endl;
            return 1;
        }
    }

    // ------------------------------------------------------------
    // Solver execution
    // ------------------------------------------------------------
    switch (config->method) {
        case Method::BISECTION:
            /* code */
            break;
        case Method::NEWTON:
            /* code */
            break;
        case Method::SECANT:
            /* code */
            break;
        case Method::FIXED_POINT:
            /* code */
            break;
        default:
            break;
    }

    return 0;
}
