#include "writer.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

Writer::Writer(Eigen::Matrix2d& vals_to_write) {
    values = vals_to_write;
}

void Writer::print_final_result() {
    std::cout << "The found root is" << values.row(values.rows() - 1)(0) << std::endl;
}

void Writer::writing_process() {
    print_final_result();
    std::unique_ptr<Printer> printer;
    choose_how(printer);
    for (auto row : values.rowwise()) {
        printer->write_values(row);
    }
}

void Writer::choose_how(std::unique_ptr<Printer>& printer) {
    int choice1 = 1;
    std::cout << "Do you want to produce a plot or to print the results somewhere?" << std::endl;
    std::cin >> choice1;
    if (choice1 == 0) {
        printer = std::make_unique<VisualPrinter>();
    }
}

VisualPrinter::VisualPrinter() {
    std::cout << "Insert the filename of the file" << std::endl;
    std::cin >> filename;
}

void VisualPrinter::write_values(Eigen::Vector2d value) {
    std::cout << value(0) << "   " << value(1) << std::endl;
}

OutputPrinter::OutputPrinter() {
    std::cout << "Here are the iterations of the method";
}

void OutputPrinter::write_values(Eigen::Vector2d value) {
    std::cout << "x = " << value(0) << "--- f(x) = " << value(1) << std::endl;
}

FilePrinter::FilePrinter() {
    std::cout << "Insert the filename of the file, without the extension" << std::endl;
}

csvPrinter::csvPrinter() {
    filename.append(".csv");
    std::cout << "Insert the separator" << std::endl;
    std::cin >> separator;
}

void csvPrinter::write_values(Eigen::Vector2d value) {
    std::cout << "x = " << value(0) << "--- f(x) = " << value(1) << std::endl;
}

datPrinter::datPrinter() {
    filename.append(".dat");
}

void datPrinter::write_values(Eigen::Vector2d value) {
    std::cout << "x = " << value(0) << "--- f(x) = " << value(1) << std::endl;
}
