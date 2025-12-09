#include <gtest/gtest.h>

#include "../../ROOT/writer.hpp"
#include "writer_base_tester.hpp"

// constructor test
TEST(WriterTest, ConstructorStoresValuesAndMethod) {
    Eigen::MatrixX2d M(2, 2);
    M << 1, 10, 2, 20;

    Writer<Eigen::MatrixX2d> w(M, WritingMethod::CSV);

    WriterBaseTester::testConstructor(w, M, WritingMethod::CSV);
}

// cli
TEST(WriterTest, BuildPrinterCLI) {
    Eigen::MatrixX2d M(1, 2);
    M << 0.0, 1.0;

    Writer<Eigen::MatrixX2d> w(M, WritingMethod::CONSOLE);

    WriterBaseTester::testBuildPrinterConsole(w);
}

// csv
TEST(WriterTest, BuildPrinterCSV) {
    Eigen::MatrixX2d M(1, 2);
    M << 0.0, 1.0;

    Writer<Eigen::MatrixX2d> w(M, WritingMethod::CSV);

    WriterBaseTester::testBuildPrinterCSV(w);
}

// dat
TEST(WriterTest, BuildPrinterDAT) {
    Eigen::MatrixX2d M(1, 2);
    M << 0.0, 1.0;

    Writer<Eigen::MatrixX2d> w(M, WritingMethod::DAT);

    WriterBaseTester::testBuildPrinterDAT(w);
}

// gnuplot
TEST(WriterTest, BuildPrinterGnuplot) {
    Eigen::MatrixX2d M(1, 2);
    M << 0.0, 1.0;

    Writer<Eigen::MatrixX2d> w(M, WritingMethod::GNUPLOT);

    WriterBaseTester::testBuildPrinterGnuplot(w);
}

// main function to run all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
