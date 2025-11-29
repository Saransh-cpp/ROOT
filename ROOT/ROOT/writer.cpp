#include "writer.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

Writer::Writer(const Eigen::MatrixX2d& vals_to_write) {
    values = vals_to_write;
}

void Writer::print_final_result() const {
    std::cout << "The found root is " << values.row(values.rows() - 1)(0) << std::endl;
}
// Add the possibility to either overwrite the file or to flush it
void Writer::writing_process() {
    print_final_result();

    std::unique_ptr<Printer> printer;
    choose_how(printer);

    for (int i = 0; i < values.rows(); i++) {
        Eigen::Vector2d row = values.row(i);
        printer->write_values(row);
    }

    // LLM
    if (auto gp = dynamic_cast<GnuplotPrinter*>(printer.get())) {
        gp->generate_gnuplot_script();
        std::cout << "Gnuplot script generated: plot.plt\n";
    }
}

void Writer::choose_how(std::unique_ptr<Printer>& printer) {
    int choice = 0;
    std::cout << "Choose output format:\n"
              << "0 - Console (verbose)\n"
              << "1 - CSV file\n"
              << "2 - DAT file\n"
              << "3 - DAT file + GNU plot" << std::endl;

    std::cin >> choice;
    if (choice == 0) {
        printer = std::make_unique<OutputPrinter>();
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
            printer = std::make_unique<csvPrinter>(fname, sep, ow_choice);
        } else if (choice == 2) {
            printer = std::make_unique<datPrinter>(fname, ow_choice);
        } else {
            printer = std::make_unique<GnuplotPrinter>(fname);
        }
    }
}

OutputPrinter::OutputPrinter() {
    std::cout << "Here are the iterations of the method";
}

void OutputPrinter::write_values(const Eigen::Vector2d& value) {
    std::cout << "x = " << value(0) << "--- f(x) = " << value(1) << std::endl;
}

FilePrinter::FilePrinter(const std::string& fname, bool ow_mode) {
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

csvPrinter::csvPrinter(const std::string& fname, char sep, bool ow_mode) : FilePrinter(fname + ".csv", ow_mode) {
    separator = sep;
}

void csvPrinter::write_values(const Eigen::Vector2d& value) {
    file << value(0) << separator << value(1) << std::endl;
}

datPrinter::datPrinter(const std::string& fname, bool ow_mode) : FilePrinter(fname + ".dat", ow_mode) {}

void datPrinter::write_values(const Eigen::Vector2d& value) {
    file << value(0) << " " << value(1) << std::endl;
}
