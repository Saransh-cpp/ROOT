#ifndef ROOT_WRITER_HPP
#define ROOT_WRITER_HPP

#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>

#include "writer_def.hpp"

template <>
void PrinterGNUPlot<Eigen::Vector2d>::generate_gnuplot_script() const;

template <>
Writer<Eigen::MatrixX2d>::Writer(const Eigen::MatrixX2d& vals_to_write, WritingMethod write_method) {
    this->values = vals_to_write;
    this->method = write_method;
}

template <>
void Writer<Eigen::MatrixX2d>::print_final_result() const {
    std::cout << "The found root is " << this->values.row(this->values.rows() - 1)(0) << std::endl;
}

template <>
void Writer<Eigen::MatrixX2d>::write(std::string filename) {
    print_final_result();

    std::unique_ptr<Printer<Eigen::Vector2d>> printer;
    choose_how(printer, filename);

    for (int i = 0; i < this->values.rows(); i++) {
        Eigen::Vector2d row = this->values.row(i);
        printer->write_values(row);
    }

    // LLM
    if (auto gp = dynamic_cast<PrinterGNUPlot<Eigen::Vector2d>*>(printer.get())) {
        gp->generate_gnuplot_script();
        std::cout << "Gnuplot script generated: plot.plt\n";
    }
}

template <typename T>
template <typename V>
void Writer<T>::choose_how(std::unique_ptr<Printer<V>>& printer, std::string filename) {
    if (this->method == WritingMethod::CONSOLE) {
        printer = std::make_unique<PrinterCLI<V>>();
    } else {
        bool ow_choice = true;
        std::cout << "Overwrite file (true) or append (false)? ";
        std::cin >> ow_choice;

        if (this->method == WritingMethod::CSV) {
            char sep;
            std::cout << "Enter the separator: ";
            std::cin >> sep;
            printer = std::make_unique<PrinterCSV<V>>(filename, sep, ow_choice);
        } else if (this->method == WritingMethod::DAT) {
            printer = std::make_unique<PrinterDAT<V>>(filename, ow_choice);
        } else {
            printer = std::make_unique<PrinterGNUPlot<V>>(filename, ow_choice);
        }
    }
}

template <typename V>
PrinterCLI<V>::PrinterCLI() {
    std::cout << "Here are the iterations of the method: " << std::endl;
}

template <>
void PrinterCLI<Eigen::Vector2d>::write_values(const Eigen::Vector2d& value) {
    std::cout << "x = " << value(0) << " --- f(x) = " << value(1) << std::endl;
}

template <typename V>
PrinterFile<V>::PrinterFile(const std::string& fname, bool ow_mode) {
    this->filename = fname;
    this->append = ow_mode;
    if (this->append) {
        file.open(this->filename, std::ios::trunc);
    } else {
        file.open(this->filename, std::ios::app);
        file << std::endl;
    }

    if (!file.is_open()) {
        throw std::runtime_error("File could not be opened");
    }
}

template <typename V>
PrinterCSV<V>::PrinterCSV(const std::string& fname, char sep, bool ow_mode) : PrinterFile<V>(fname + ".csv", ow_mode) {
    this->separator = sep;
}

template <>
void PrinterCSV<Eigen::Vector2d>::write_values(const Eigen::Vector2d& value) {
    file << value(0) << this->separator << value(1) << std::endl;
}

template <typename V>
PrinterDAT<V>::PrinterDAT(const std::string& fname, bool ow_mode) : PrinterFile<V>(fname + ".dat", ow_mode) {}

template <>
void PrinterDAT<Eigen::Vector2d>::write_values(const Eigen::Vector2d& value) {
    file << value(0) << " " << value(1) << std::endl;
}

template <typename V>
PrinterGNUPlot<V>::PrinterGNUPlot(const std::string& fname, bool ow_mode) : PrinterDAT<V>(fname, ow_mode) {}

template <>
void PrinterGNUPlot<Eigen::Vector2d>::generate_gnuplot_script() const {
    std::string base_name = this->filename;
    size_t last_dot = base_name.find_last_of(".");
    if (last_dot != std::string::npos) {
        base_name = base_name.substr(0, last_dot);
    }

    std::string png_file = base_name + ".png";  // PNG output
    std::string plt_file = base_name + ".plt";  // Gnuplot script

    std::ofstream script(plt_file);
    if (!script.is_open()) {
        std::cerr << "Error: could not open gnuplot script file.\n";
        return;
    }
    // LLM
    script << "# Auto-generated gnuplot script\n"
           << "set terminal pngcairo size 1000,800 enhanced font 'Arial,12'\n"
           << "set output '" << png_file << "'\n"
           << "set title 'Root-Finding Iterations'\n"
           << "set xlabel 'x'\n"
           << "set ylabel 'f(x)'\n"
           << "set grid\n"
           << "plot '" << this->filename
           << "' using 1:2 with linespoints lt rgb 'blue' pt 7 lw 2 title 'Iteration Path'\n";

    script.close();

    // Check if gnuplot exists before calling it
    if (std::system("which gnuplot > /dev/null 2>&1") == 0) {
        std::system(("gnuplot " + plt_file).c_str());
        std::cout << "Gnuplot image generated: " << png_file << std::endl;
    } else {
        std::cerr << "Warning: gnuplot not found. Script generated but PNG not created.\n";
    }
}

#endif  // ROOT_WRITER_IMPL_HPP
