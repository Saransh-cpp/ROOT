#include "writer.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <Eigen/Dense>

template <>
void GnuplotPrinter<Eigen::Vector2d>::generate_gnuplot_script() const;

template <>
Writer<Eigen::MatrixX2d>::Writer(const Eigen::MatrixX2d& vals_to_write) {
    values = vals_to_write;
}

template <>
void Writer<Eigen::MatrixX2d>::print_final_result() const {
    std::cout << "The found root is " << values.row(values.rows() - 1)(0) << std::endl;
}

template <>
void Writer<Eigen::MatrixX2d>::writing_process() {
    print_final_result();

    std::unique_ptr<Printer<Eigen::Vector2d>> printer;
    choose_how(printer);

    for (int i = 0; i < values.rows(); i++) {
        Eigen::Vector2d row = values.row(i);
        printer->write_values(row);
    }

    // LLM
    if (auto gp = dynamic_cast<GnuplotPrinter<Eigen::Vector2d>*>(printer.get())) {
        gp->generate_gnuplot_script();
        std::cout << "Gnuplot script generated: plot.plt\n";
    }
}

template <typename T>
template <typename V>
void Writer<T>::choose_how(std::unique_ptr<Printer<V>>& printer) {
    int choice = 0;
    std::cout << "Choose output format:\n"
              << "0 - Console (verbose)\n"
              << "1 - CSV file\n"
              << "2 - DAT file\n"
              << "3 - DAT file + GNU plot" << std::endl;

    std::cin >> choice;
    if (choice == 0) {
        printer = std::make_unique<OutputPrinter<V>>();
    } else {
        std::string fname;
        std::cout << "Insert the filename (w/o) extension: ";
        std::cin >> fname;

        bool ow_choice = true;
        std::cout << "Overwrite file (true) or append (false)? ";
        std::cin >> ow_choice;

        if (choice == 1) {
            char sep;
            std::cout << "Enter the separator: ";
            std::cin >> sep;
            printer = std::make_unique<csvPrinter<V>>(fname, sep, ow_choice);
        } else if (choice == 2) {
            printer = std::make_unique<datPrinter<V>>(fname, ow_choice);
        } else {
            printer = std::make_unique<GnuplotPrinter<V>>(fname, ow_choice);
        }
    }
}

template <typename V>
OutputPrinter<V>::OutputPrinter() {
    std::cout << "Here are the iterations of the method";
}

template <>
void OutputPrinter<Eigen::Vector2d>::write_values(const Eigen::Vector2d& value) {
    std::cout << "x = " << value(0) << "--- f(x) = " << value(1) << std::endl;
}

template <typename V>
FilePrinter<V>::FilePrinter(const std::string& fname, bool ow_mode) {
    filename = fname;
    append = ow_mode;
    if (ow_mode) {
        file.open(filename, std::ios::trunc);
    } else {
        file.open(filename, std::ios::app);
    }

    if (!file.is_open()) {
        throw std::runtime_error("File could not be opened");
    }
}

template <typename V>
csvPrinter<V>::csvPrinter(const std::string& fname, char sep, bool ow_mode) : FilePrinter<V>(fname + ".csv", ow_mode) {
    separator = sep;
}

template <>
void csvPrinter<Eigen::Vector2d>::write_values(const Eigen::Vector2d& value) {
    file << value(0) << separator << value(1) << std::endl;
}

template <typename V>
datPrinter<V>::datPrinter(const std::string& fname, bool ow_mode) : FilePrinter<V>(fname + ".dat", ow_mode) {}

template <>
void datPrinter<Eigen::Vector2d>::write_values(const Eigen::Vector2d& value) {
    file << value(0) << " " << value(1) << std::endl;
}

template <typename V>
GnuplotPrinter<V>::GnuplotPrinter(const std::string& fname, bool ow_mode) : datPrinter<V>(fname, ow_mode) {}

template <>
void GnuplotPrinter<Eigen::Vector2d>::generate_gnuplot_script() const {
    std::ofstream script("plot.plt");
    script << "set terminal pngcairo size 800,600\n"
           << "set output 'plot.png'\n"
           << "plot '" << filename << "' using 1:2 with linespoints\n";
    script.close();

    std::system("gnuplot plot.plt");
}
