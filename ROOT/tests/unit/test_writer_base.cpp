#include <gtest/gtest.h>

#include "writer_base_tester.hpp"

TEST_F(WriterBaseTester, BuildPrinterCLI) {
    Eigen::MatrixX2d values(3, 2);
    values << 0, 1, 1, 0, 2, -1;

    std::unique_ptr<PrinterBase<Eigen::Vector2d>> printer;
    this->testBuildPrinter(values, WritingMethod::CONSOLE, printer);
}

TEST_F(WriterBaseTester, BuildPrinterCSV) {
    Eigen::MatrixX2d values(3, 2);
    values << 0, 1, 1, 0, 2, -1;

    std::unique_ptr<PrinterBase<Eigen::Vector2d>> printer;
    this->testBuildPrinter(values, WritingMethod::CSV, printer);
}

TEST_F(WriterBaseTester, BuildPrinterDAT) {
    Eigen::MatrixX2d values(3, 2);
    values << 0, 1, 1, 0, 2, -1;

    std::unique_ptr<PrinterBase<Eigen::Vector2d>> printer;
    this->testBuildPrinter(values, WritingMethod::DAT, printer);
}

TEST_F(WriterBaseTester, BuildPrinterGNUPLOT) {
    Eigen::MatrixX2d values(3, 2);
    values << 0, 1, 1, 0, 2, -1;

    std::unique_ptr<PrinterBase<Eigen::Vector2d>> printer;
    this->testBuildPrinter(values, WritingMethod::GNUPLOT, printer);
}

TEST_F(WriterBaseTester, WriteCLI) {
    Eigen::MatrixX2d values(3, 2);
    values << 0, 1, 1, 0, 2, -1;

    this->testWrite(values, WritingMethod::CONSOLE);
}

TEST_F(WriterBaseTester, WriteCSV) {
    Eigen::MatrixX2d values(3, 2);
    values << 0, 1, 1, 0, 2, -1;

    this->testWrite(values, WritingMethod::CSV);
}

TEST_F(WriterBaseTester, WriteDAT) {
    Eigen::MatrixX2d values(3, 2);
    values << 0, 1, 1, 0, 2, -1;

    this->testWrite(values, WritingMethod::DAT);
}

TEST_F(WriterBaseTester, WriteGNUPLOT) {
    Eigen::MatrixX2d values(3, 2);
    values << 0, 1, 1, 0, 2, -1;

    this->testWrite(values, WritingMethod::GNUPLOT);
}
