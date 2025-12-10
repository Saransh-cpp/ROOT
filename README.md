# ROOT

[![Build and Install](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-and-install.yml/badge.svg?branch=main)](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-and-install.yml)
[![Unit tests](https://github.com/Saransh-cpp/ROOT/actions/workflows/unit-tests.yml/badge.svg?branch=main)](https://github.com/Saransh-cpp/ROOT/actions/workflows/unit-tests.yml)
[![Documentation Deployment](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-and-deploy-docs.yml/badge.svg?branch=main)](https://saransh-cpp.github.io/ROOT)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/Saransh-cpp/ROOT/main.svg)](https://results.pre-commit.ci/latest/github/Saransh-cpp/ROOT/main)

ROOT, but not the [particle physics one](https://github.com/root-project/root). Project submission for MATH-458: Programming concepts in scientific computing.

## Project structure and dependencies

The project uses the recommended [Canonical Project Structure](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html) for C++ projects.

```
.
├── .clang-format             # ClangFormat configuration
├── .clang-tidy               # ClangTidy configuration
├── .github                   # GitHub related files
│   └── workflows             # CI files to build/test/check the project
├── .gitignore                # To ignore files from git
├── .gitmodules
├── .pre-commit-config.yaml   # Pre-commit configuration
├── CMakeLists.txt            # Top-level CMakeLists.txt
├── Doxyfile                  # Doxygen configuration
├── README.md                 # This file
├── ROOT                      # User-facing root_cli application/executable
│   ├── CMakeLists.txt        # CMakeLists.txt for user-facing root_cli application/executable
│   ├── ROOT                  # CXX and HXX files
│   └── tests                 # Tests for the root_cli application/executable
├── external                  # External libraries (as git submodules)
│   ├── CLI11                 # CLI11 submodule for the root_cli executable
│   ├── eigen                 # Eigen submodule for matrix and vector calculations
│   └── googletest            # Googletest submodule to run tests
└── libROOT                   # Developer-facing ROOT library
    ├── CMakeLists.txt        # CMakeLists.txt for developer-facing ROOT library
    ├── libROOT               # CXX and HXX files
    └── tests                 # Tests for the ROOT library
```

### Dependencies

#### Dependencies for the project

##### Required

The required dependencies are included within the project as git submodules and are pinned to specific
versions for reproducibility.

- `CLI11` (`v2.6.1`): for constructing the CLI interface for the user-facing `root_cli` application.
- `Eigen3` (`v5.0.1`): for matric and vector usage / calculations.

##### Optional

These can be installed by a user and are not installed through the project's build system.

- `gnuplot`: for plotting results

#### Required dependencies for the tests

`gnuplot` must be installed before building the project with `-DTEST=ON`. `GoogleTest` is installed automatically if the project is built with `-DTEST=ON`.

- `GoogleTest` (`v1.17.0`): for all tests.
- `gnuplot`: for testing `gnuplot` related code.

#### Dependencies for the documentation

These can be installed by a user and are not installed through the project's build system.

##### Required

- `doxygen`: for generating the documentation.

##### Optional

- `graphviz`: for generating hierarchy and flow diagrams in the documentation.


## Building and installing the project

To build the project, clone the repository locally:

```
git clone https://github.com/Saransh-cpp/ROOT --recursive
```

And build the library and the application using:

```
cmake -S. -Bbuild
cmake --build build
```

The library (`libROOT` - header-only) and the application (`root_cli`) can be installed using:

```
cmake -S. -DCMAKE_INSTALL_PREFIX=<installation_directory_such_as_/usr/local/> -Bbuild
cmake --build build --target install
```

Which will put the library's header files and the application `<install_path>/include/libROOT`,  and `<install_path>/bin` respectively.

## Usage

TLDR;
```
ROOT Command Line Interface


root_cli [OPTIONS] SUBCOMMAND


OPTIONS:
  -h,     --help              Print this help message and exit
  -v,     --verbose           Enable verbose output
          --wcli, --write-to-cli
                              Write results to command line
          --wcsv, --write-to-csv TEXT
                              Path for writing results to CSV file
          --ocsvsep, --output-csv-sep CHAR [,]
                              Separator character for CSV output
          --wdat, --write-to-dat TEXT
                              Path for writing results to DAT file
          --wgnuplot, --write-to-gnuplot TEXT
                              Path for writing results to Gnuplot file
          --ofmode, --output-file-mode CHAR:{a,o} [o]
                              Append or overwrite output file: 'a' for append, 'o' for
                              overwrite

SUBCOMMANDS:
  csv                         Use CSV input
  dat                         Use DAT input
  cli                         Use CLI input
```

Assuming that `root_cli` is on your `PATH`, the program can be executed with:

```
root_cli <arguments>
# or use the full install/build path of the executable: /path/to/root_cli <arguments>
```

In order to print out more information about the arguments and the subcommands:

```
$ ./build/ROOT/ROOT/root_cli <arguments>
```
Every additional needed function must be added together with the function to find the root of.
Here's a list of examples of possible execution syntax:

- CLI input, CLI output, Newton-Raphson method to find the root of x^2-4, starting from initial guess 1 (default tolerance and maximum iterations):

```
$ ./build/ROOT/ROOT/root_cli --wcli cli --function "x^2-4" newton --initial 1.0 --derivative "2*x"
```

- DAT input file called input.dat with first row not being header and " " separating different values, .dat file output called output.dat, Bisection method to find the root of x^3-1, with initial interval [-2,2], verbose output (given tolerance and maximum iterations):

    ```
    root_cli --verbose --wdat output dat input
    ```

    where input.dat is:

    ```
    function = x^3-1
    method = bisection
    initial = -1
    tolerance = 1e-5
    max-iterations = 100
    derivative = 2*x
    ```

- CSV input file called input.csv with first row which is a header and "," separating different values, .csv file ouput
called output.csv, Fixed Point Method to find the root of cos(x), with
initial guess 0.5, fixed point function cos(x):

    ```
    root_cli --wcsv output --ocsvsep , csv input --sep , --header
    ```

    where input.csv is:

    ```
    function,method,initial,tolerance,max_iterations,g-function
    'cos(x)',fixed_point,0.5,1e-5,100,'cos(x)'
    ```

- CLI input, .dat output file called output.dat and moreover a GNU Plot is created from it as output.png. Chords method to solve
the equation x^3-8 starting from the two initial points 1 and 3:

    ```
    root_cli --wdat output --wgnuplot output cli --function x^3-8 chords --x0 --x1 3
    ```

The installed CLI application can simply be used by:

```
$ <install_path>/bin/root_cli
# or just root_cli if installed in /usr/local/bin/ on unix for instance
```

And the shared library can be used inside `cxx` files using:

```
# pass the path of headers
g++ <file>.cpp -o <executable_name> -I<install_path>/include
```

All of which can also be set in `CMakeLists.txt`.

## Tests

Tests for the library (`libROOT`) can be found in `libROOT/tests`, and the tests for the application (`root_cli`) can be found in `ROOT/tests`. We follow [test-driven development (TDD)](https://en.wikipedia.org/wiki/Test-driven_development), which means that each code changing PR must have tests to validate the changes.

All the following tests and checks run as part of a Continuous Integration pipeline on GitHub Actions (on every PR and push to `main`).

### Structure of tests

```
./ROOT/tests                                # Tests for ROOT
├── integration                             # Integration tests for ROOT
│   ├── CMakeLists.txt                      # Build file for Integration tests
│   └── test_cli.cpp                        # Tests for the entire root_cli application
├── test_data                               # Data files used in tests
│   ├── config.csv
│   └── config.dat
└── unit                                    # Unit tests for ROOT
    ├── CMakeLists.txt                      # Build file for unit tests
    ├── function_parser_base_tester.hpp     # The parameterized testing class (friend of the class being tested)
    ├── polynomial_parser_tester.hpp
    ├── reader_base_tester.hpp
    ├── reader_csv_tester.hpp
    ├── test_function_parser.cpp            # Actual tests (calling paramaterized functions from the testing class)
    ├── test_polynomial_parser.cpp
    ├── test_reader.cpp
    ├── test_trigonometric_parser.cpp
    ├── test_writer.cpp
    ├── trigonometric_parser_tester.hpp
    └── writer_tester.hpp
...
./libROOT/tests                             # Tests for libROOT
└── unit                                    # Unit tests for libROOT
    ├── CMakeLists.txt                      # Build file for unit tests
    ├── solver_tester.hpp                   # The parameterized testing class (friend of the class being tested)
    └── test_solver.cpp                     # Actual tests (calling paramaterized functions from the testing class)
```

## Building and running tests

To build the tests, make sure your submodules are up-to-date (more specifically, the `googletest` submodule):

```
git submodule update --init
```

And build the code with `-DTEST=ON`:

```
cmake -S. -Bbuild -DTEST=ON
cmake --build build
```

To run the tests, use the `ctest` command:

```
ctest --test-dir build --output-on-failure
```

### Linters and formatters

#### Clang Tidy

The project uses `clang-tidy` to lint C++ and related files (except test files).

One can generate make files with `clang-tidy` enabled which will enable cmake to run `clang-tidy` along while compiling the files.

```
cmake -S. -Bbuild -DCMAKE_CXX_CLANG_TIDY=clang-tidy
cmake --build build -j8
```

#### Pre-commit

The project also uses a set of `pre-commit` hooks (including ClangFormat and generic linters and formatters) to perform static analysis and formatting on the code. The hooks can be installed locally using:

```bash
pre-commit install  # inside your environment
```

This would run the checks every time a commit is created locally. The checks will only run on the files modified by that commit, but the checks can be triggered for all the files using:

```bash
pre-commit run --all-files
```

If you would like to skip the failing checks and push the code for further discussion, use the `--no-verify` option with `git commit`.

## Documentation

The project uses Doxygen to document the code. The project also provides CMake integration for building documentation.

As with the tests, each code change should be accompanied with an updated documentation.

### Building docs locally

One can build the documentation locally using:

```
cmake -S . -B build
cmake --build build --target doc
```

which will write the HTML files to `docs/html`.

### Building docs on GH Pages

The documentation is automatically built (on every PR) and deployed (on every push to `main`) to GH Pages using the `build-and-deploy-docs` workflow.
