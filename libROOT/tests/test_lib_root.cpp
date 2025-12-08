#include <gtest/gtest.h>

#include "libROOT/solver.hpp"

#include <gtest/gtest.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <array>
#include <sstream>

// TO DO:
// Right now the tests are just checking whether the root is near its computed respective within a given tolerance,
// but actually the solver stops even if f(x) is below the tolerance, so these tests are not complete
// -> BUG TO FIX LATER BY ADDING MORE THINGS TO TEST IN EACH TESTER

//LLM helper to read from CLI
static std::string exec_command(const std::string& cmd) {
    std::array<char, 256> buffer{};
    std::string result;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");

    while (fgets(buffer.data(), buffer.size(), pipe)) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}

TEST(NewtonMethod, QuadraticConvergesToMinus2) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function x^2-4"
        " newton"
        " --initial -1"
        " --derivative 2*x";

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    // Look for the line: "The found root is -2"
    const std::string token = "The found root is ";
    auto pos = output.find(token);

    ASSERT_NE(pos, std::string::npos)
        << "Output did not contain expected root line.\n"
        <<  "Output was:\n"
        << output;

    pos += token.size();

    // Extract the number after the token -> LLM
    std::stringstream ss(output.substr(pos));
    double root = 0.0;
    ss >> root;

    // Validate the root value
    EXPECT_NEAR(root, -2.0, 1e-4);
}

TEST(NewtonMethod, DerivativeZeroError) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function x^3-3"
        " newton"
        " --initial 0"
        " --derivative 3*x^2"
        " 2>&1";   // redirect stderr to stdout

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    // Exact expected output:
    std::string expectedError =
        "Caught error: Division by 0. The method will diverge";

    // Check that the output contains the message (ignoring color codes)
    ASSERT_NE(output.find(expectedError), std::string::npos)
        << "Expected error message not found.\nOutput was:\n"
        << output;
}

TEST(NewtonMethod, SinusoidConvergesToZero) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function 'sin(x)'"
        " newton"
        " --initial 1"
        " --derivative 'cos(x)'";

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    const std::string token = "The found root is ";
    auto pos = output.find(token);

    ASSERT_NE(pos, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output;

    pos += token.size();

    // Extract the number after the token
    std::stringstream ss(output.substr(pos));
    double root = 0.0;
    ss >> root;

    // Validate the root value
    EXPECT_NEAR(root, 0.0, 1e-4);
}

TEST(BisectionMethod, QuadraticConvergesToOne) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function x^2-1"
        " bisection"
        " --interval_a 0 --interval_b 2";

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    const std::string token = "The found root is ";
    auto pos = output.find(token);

    ASSERT_NE(pos, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output;

    pos += token.size();

    // Extract the number after the token
    std::stringstream ss(output.substr(pos));
    double root = 0.0;
    ss >> root;

    // Validate the root value
    EXPECT_NEAR(root, 1.0, 1e-4);
}

TEST(BisectionMethod, InvalidInterval) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function x^2+1"
        " bisection"
        " --interval_a -1 --interval_b 1"
        " 2>&1";   // redirect stderr to stdout

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    // Your exact expected output:
    std::string expectedError =
        "Caught error: For Bisection method, function values at initial points must have opposite signs.";

    // Check that the output contains the message (ignoring ANSI color codes)
    ASSERT_NE(output.find(expectedError), std::string::npos)
        << "Expected error message not found.\nOutput was:\n"
        << output;
}

TEST(ChordsMethod, CubicConvergesToTwo) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function x^3-8"
        " chords"
        " --x0 1 --x1 3";

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    const std::string token = "The found root is ";
    auto pos = output.find(token);

    ASSERT_NE(pos, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output;

    pos += token.size();

    // Extract the number after the token
    std::stringstream ss(output.substr(pos));
    double root = 0.0;
    ss >> root;

    // Validate the root value
    EXPECT_NEAR(root, 2.0, 1e-4);
}

TEST(ChordsMethod, DivisionByZeroChords) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function x^2-4"
        " chords"
        " --x0 1 --x1 1"
        " 2>&1";   // redirect stderr to stdout

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    // Your exact expected output:
    std::string expectedError =
        "Caught error: Division by 0. The method will diverge";

    // Check that the output contains the message (ignoring ANSI color codes)
    ASSERT_NE(output.find(expectedError), std::string::npos)
        << "Expected error message not found.\nOutput was:\n"
        << output;
}

TEST(FixedPointMethod, CosineConvergence) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function 'cos(x)'"
        " fixed_point"
        " --initial 0.5"
        " --g-function 'cos(x)'";

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    const std::string token = "The found root is ";
    auto pos = output.find(token);

    ASSERT_NE(pos, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output;

    pos += token.size();

    // Extract the number after the token
    std::stringstream ss(output.substr(pos));
    double root = 0.0;
    ss >> root;

    // Validate the root value
    EXPECT_NEAR(root, 0.739, 1e-4);
}

TEST(FixedPointMethod, DivergentFP) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function 'cos(x)'"
        " fixed_point"
        " --initial 0"
        " --g-function 2*x+3"
        " 2>&1";   // redirect stderr to stdout

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    // Your exact expected output:
    std::string expectedError =
        "The solution did not converge in";

    // Check that the output contains the message (ignoring ANSI color codes)
    ASSERT_NE(output.find(expectedError), std::string::npos)
        << "Expected error message not found.\nOutput was:\n"
        << output;
}

TEST(FixedPointMethod, AitkenSpeed) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd1 =
        exe +
        " --wcli --verbose cli"
        " --function 'cos(x)'"
        " fixed_point"
        " --initial 0.5"
        " --g-function 'cos(x)'";

    std::string cmd2 =
        exe +
        " --wcli --verbose cli"
        " --function 'cos(x)'"
        " --aitken"
        " fixed_point"
        " --initial 0.5"
        " --g-function 'cos(x)'";

    std::string output1 = exec_command(cmd1);
    std::string output2 = exec_command(cmd2);

    ASSERT_FALSE(output1.empty());
    ASSERT_FALSE(output2.empty());

    const std::string token = "Converged in ";
    auto pos1 = output1.find(token);
    auto pos2 = output2.find(token);

    ASSERT_NE(pos1, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output1;

    ASSERT_NE(pos2, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output2;

    pos1 += token.size();
    pos2 += token.size();

    // Extract the number after the token
    std::stringstream ss1(output1.substr(pos1));
    int iter1 = 0;
    ss1 >> iter1;

    std::stringstream ss2(output2.substr(pos2));
    int iter2 = 0;
    ss2 >> iter2;

    EXPECT_TRUE(iter2 < iter1);
}

TEST(FixedPointMethod, ToleranceEffectiveness) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd1 =
        exe +
        " --wcli --verbose cli"
        " --function 'cos(x)'"
        " --tolerance 0.001"
        " fixed_point"
        " --initial 0.5"
        " --g-function 'cos(x)'";

    std::string cmd2 =
        exe +
        " --wcli --verbose cli"
        " --function 'cos(x)'"
        " --tolerance 0.01"
        " fixed_point"
        " --initial 0.5"
        " --g-function 'cos(x)'";

    std::string output1 = exec_command(cmd1);
    std::string output2 = exec_command(cmd2);

    ASSERT_FALSE(output1.empty());
    ASSERT_FALSE(output2.empty());

    const std::string token = "Converged in ";
    auto pos1 = output1.find(token);
    auto pos2 = output2.find(token);

    ASSERT_NE(pos1, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output1;

    ASSERT_NE(pos2, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output2;

    pos1 += token.size();
    pos2 += token.size();

    // Extract the number after the token
    std::stringstream ss1(output1.substr(pos1));
    int iter1 = 0;
    ss1 >> iter1;

    std::stringstream ss2(output2.substr(pos2));
    int iter2 = 0;
    ss2 >> iter2;

    EXPECT_TRUE(iter2 < iter1);
}

TEST(NewtonMethod, SlowConvergence) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function x^2"
        " newton"
        " --initial -15"
        " --derivative 2*x";

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    // Look for the line: "The found root is 0"
    const std::string token = "The found root is ";
    auto pos = output.find(token);

    ASSERT_NE(pos, std::string::npos)
        << "Output did not contain expected root line.\n"
        <<  "Output was:\n"
        << output;

    pos += token.size();

    std::stringstream ss(output.substr(pos));
    double root = 0.0;
    ss >> root;

    // Validate the root value
    EXPECT_NEAR(root, 0.0, 1e-2);
}