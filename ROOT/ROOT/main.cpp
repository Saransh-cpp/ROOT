#include <CLI/CLI.hpp>
#include <Eigen/Dense>
#include <functional>
#include <iostream>
#include <libROOT/solver.hpp>
#include <memory>
#include <string>

#include "config.hpp"
#include "function_parser.hpp"
#include "reader.hpp"
#include "writer.hpp"

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

    bool verbose = false;
    app.add_flag("-v,--verbose", verbose, "Enable verbose output")->capture_default_str();

    bool write_to_cli = false;
    app.add_flag("--wcli,--write-to-cli", write_to_cli, "Write results to command line")->capture_default_str();

    std::string write_to_csv;
    char w_csv_sep = ',';
    app.add_option("--wcsv,--write-to-csv", write_to_csv, "Path for writing results to CSV file");
    app.add_option("--ocsvsep,--output-csv-sep", w_csv_sep, "Separator character for CSV output")
        ->capture_default_str();

    std::string write_to_dat;
    app.add_option("--wdat,--write-to-dat", write_to_dat, "Path for writing results to DAT file");
    std::string write_with_gnuplot;
    app.add_option("--wgnuplot,--write-to-gnuplot", write_with_gnuplot, "Path for writing results to Gnuplot file");

    char append_or_overwrite = 'o';
    app.add_option("--ofmode,--output-file-mode", append_or_overwrite,
                   "Append or overwrite output file: 'a' for append, 'o' for overwrite")
        ->check(CLI::IsMember({'a', 'o'}))
        ->capture_default_str();

    // Subcommands for different input methods
    // CSV
    auto* csv = app.add_subcommand("csv", "Use CSV input");
    std::string csv_file;
    csv->add_option("--file", csv_file, "Path to CSV file containing input data")->required()->check(CLI::ExistingFile);

    char csv_sep = ',';
    csv->add_option("--sep", csv_sep, "Separator character for CSV file")->capture_default_str();
    char csv_quote = '"';
    csv->add_option("--quote", csv_quote, "Quote/delimiter character for CSV file")->capture_default_str();
    bool csv_header = true;
    csv->add_option("--header", csv_header, "Indicates whether the first row is a header row")->capture_default_str();

    // DAT
    auto* dat = app.add_subcommand("dat", "Use DAT input");
    std::string dat_file;
    dat->add_option("--file", dat_file, "Path to DAT file containing input data")->required()->check(CLI::ExistingFile);

    // CLI
    auto* cli = app.add_subcommand("cli", "Use CLI input");
    std::string function_str;
    cli->add_option("-f,--function", function_str,
                    "Function to find root of (only polynomial and simple trig expressions)")
        ->required();

    bool aitken = false;
    cli->add_flag("-a,--aitken", aitken, "Enable Aitken acceleration")->capture_default_str();

    double tolerance = 1e-5;
    cli->add_option("-t,--tolerance", tolerance, "Tolerance for convergence")
        ->check(CLI::PositiveNumber)
        ->capture_default_str();

    int max_iterations = 100;
    cli->add_option("-n,--max-iterations", max_iterations, "Maximum number of iterations")
        ->check(CLI::PositiveNumber)
        ->capture_default_str();

    cli->require_subcommand(1);

    // Subcommands for the CLI subcommand
    // newton
    auto* newton = cli->add_subcommand("newton", "Use Newton's method");
    double newton_initial = 0.0;
    std::string derivative_function;
    newton->add_option("--initial", newton_initial, "Initial guess x0 for Newton's method")->required();
    newton->add_option("--derivative", derivative_function, "Derivative of the function (optional)")->required();

    // chords
    auto* chords = cli->add_subcommand("chords", "Use Chords method");
    double chords_x0 = 0.0;
    double chords_x1 = 1.0;
    chords->add_option("--x0", chords_x0, "First initial guess x0")->required();
    chords->add_option("--x1", chords_x1, "Second initial guess x1")->required();

    // fixed-point
    auto* fixed_point = cli->add_subcommand("fixed_point", "Use fixed-point iterative method");
    double fixed_point_initial = 0.0;
    std::string function_g;
    fixed_point->add_option("--initial", fixed_point_initial, "Initial guess x0 for foxed point method")->required();
    fixed_point->add_option("--g-function", function_g, "g(x) for fixed-point iteration")->required();

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
    // Initialise pointers nand variables
    // ------------------------------------------------------------
    std::unique_ptr<ConfigBase> config;
    std::unique_ptr<ReaderBase> reader;
    Eigen::MatrixX2d results;

    // ------------------------------------------------------------
    // Reader execution
    // ------------------------------------------------------------
    if (*csv) {
        reader = std::make_unique<ReaderCSV>();
        config = reader->read(csv, verbose);
    } else if (*dat) {
        reader = std::make_unique<ReaderDAT>();
        config = reader->read(dat, verbose);
    } else if (*cli) {
        reader = std::make_unique<ReaderCLI>();
        config = reader->read(cli, verbose);
    }

    // ------------------------------------------------------------
    // Solver execution
    // ------------------------------------------------------------
    switch (config->method) {
        case Method::BISECTION: {
            Eigen::Vector2d interval = {dynamic_cast<BisectionConfig*>(config.get())->initial_point,
                                        dynamic_cast<BisectionConfig*>(config.get())->final_point};
            Solver solver(config->function, interval, config->method, config->max_iterations, config->tolerance,
                          config->aitken, config->verbose);
            results = solver.solve();
            break;
        }
        case Method::NEWTON: {
            Solver solver(config->function, dynamic_cast<NewtonConfig*>(config.get())->initial_guess, config->method,
                          config->max_iterations, config->tolerance, config->aitken, config->verbose,
                          dynamic_cast<NewtonConfig*>(config.get())->derivative);
            results = solver.solve();
            break;
        }
        case Method::CHORDS: {
            Eigen::Vector2d interval = {dynamic_cast<ChordsConfig*>(config.get())->initial_point1,
                                        dynamic_cast<ChordsConfig*>(config.get())->initial_point2};
            Solver solver(config->function, interval, config->method, config->max_iterations, config->tolerance,
                          config->aitken, config->verbose);
            results = solver.solve();
            break;
        }
        case Method::FIXED_POINT: {
            Solver solver(config->function, dynamic_cast<FixedPointConfig*>(config.get())->initial_guess,
                          config->method, config->max_iterations, config->tolerance, config->aitken, config->verbose,
                          dynamic_cast<FixedPointConfig*>(config.get())->g_function);
            results = solver.solve();
            break;
        }
        default:
            break;
    }

    // ------------------------------------------------------------
    // Writer execution
    // ------------------------------------------------------------
    if (write_to_cli) {
        Writer<Eigen::MatrixX2d> writer(results, WritingMethod::CONSOLE);
        writer.write();
    } else if (!write_to_csv.empty()) {
        Writer<Eigen::MatrixX2d> writer(results, WritingMethod::CSV, write_to_csv, w_csv_sep,
                                        append_or_overwrite == 'o');
        writer.write();
    } else if (!write_to_dat.empty()) {
        Writer<Eigen::MatrixX2d> writer(results, WritingMethod::DAT, write_to_dat, ' ', append_or_overwrite == 'o');
        writer.write();
    } else if (!write_with_gnuplot.empty()) {
        Writer<Eigen::MatrixX2d> writer(results, WritingMethod::GNUPLOT, write_with_gnuplot, ',',
                                        append_or_overwrite == 'o');
        writer.write();
    }

    return 0;
}
