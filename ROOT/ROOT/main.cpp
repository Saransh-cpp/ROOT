#include <CLI/CLI.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "config.hpp"
#include "function_parser.hpp"
#include "reader.hpp"

int main(int argc, char** argv) {
    // ------------------------------------------------------------
    // Command-line interface
    // ------------------------------------------------------------

    // !!!!!!!!!!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!!!!!
    // It is required to add options and subcommands in the main
    // function itself. CLI11 raises either segmentation faults or
    // bus errors if options/subcommands are added from other
    // functions. Hence, we cannot modularize this part of the code.
    // !!!!!!!!!!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!!!!!

    CLI::App app{"ROOT Command Line Interface"};
    app.require_subcommand(1);

    // Global options shared across methods
    bool verbose = false;
    app.add_flag("-v,--verbose", verbose, "Enable verbose output (applies to all methods)");

    // Subcommands for different input methods
    // CSV
    auto* csv = app.add_subcommand("csv", "Use CSV input");
    std::string csv_file;
    csv->add_option("--file", csv_file, "Path to CSV file containing input data")->required();

    // DAT
    auto* dat = app.add_subcommand("dat", "Use DAT input");
    std::string dat_file;
    dat->add_option("--file", dat_file, "Path to DAT file containing input data")->required();

    // CLI
    auto* cli = app.add_subcommand("cli", "Use CLI input");
    std::string function_str;
    cli->add_option("-f,--function", function_str,
                    "Function to find root of (only polynomial and simple trig expressions)")
        ->required();

    bool aitken = false;
    cli->add_flag("-a,--aitken", aitken, "Enable Aitken acceleration");

    double tolerance = 1e-5;
    cli->add_option("-t,--tolerance", tolerance, "Tolerance for convergence")->check(CLI::PositiveNumber);

    int max_iterations = 100;
    cli->add_option("-n,--max-iterations", max_iterations, "Maximum number of iterations")->check(CLI::PositiveNumber);

    cli->require_subcommand(1);

    // Subcommands for the CLI subcommand
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

    // !!!!!!!!!!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Everything below here can be modularized into functions and
    // classes as needed since CLI11 has finished all parsing of
    // options and subcommands.
    // !!!!!!!!!!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // ------------------------------------------------------------
    // Solver configuration
    // ------------------------------------------------------------

    std::unique_ptr<ConfigBase> config;
    std::unique_ptr<ReaderBase> reader;

    // Read configuration based on selected input method
    if (*csv) {
        reader = std::make_unique<ReaderCSV>();
        config = reader->read(csv);
    } else if (*dat) {
        reader = std::make_unique<ReaderDAT>();
        config = reader->read(dat);
    } else if (*cli) {
        reader = std::make_unique<ReaderCLI>();
        config = reader->read(cli);
    }

    // ------------------------------------------------------------
    // Solver execution
    // ------------------------------------------------------------
    switch (config->method) {
        case Method::BISECTION:
            std::cout << config->max_iterations << std::endl;
            break;
        case Method::NEWTON:
            std::cout << config->function(0) << std::endl;
            std::cout << dynamic_cast<NewtonConfig*>(config.get())->initial_guess << std::endl;
            std::cout << dynamic_cast<NewtonConfig*>(config.get())->derivative(3) << std::endl;
            break;
        case Method::SECANT:
            std::cout << config->function(0) << std::endl;
            std::cout << dynamic_cast<SecantConfig*>(config.get())->final_point << std::endl;
            break;
        case Method::FIXED_POINT:
            std::cout << config->function(0) << std::endl;
            std::cout << dynamic_cast<FixedPointConfig*>(config.get())->initial_guess << std::endl;
            break;
        default:
            break;
    }

    return 0;
}
