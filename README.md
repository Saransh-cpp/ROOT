# ROOT

[![Build and Install](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-and-install.yml/badge.svg?branch=main)](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-and-install.yml)
[![Unit tests](https://github.com/Saransh-cpp/ROOT/actions/workflows/unit-tests.yml/badge.svg?branch=main)](https://github.com/Saransh-cpp/ROOT/actions/workflows/unit-tests.yml)
[![Documentation Deployment](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-and-deploy-docs.yml/badge.svg?branch=main)](https://saransh-cpp.github.io/ROOT)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/Saransh-cpp/ROOT/main.svg)](https://results.pre-commit.ci/latest/github/Saransh-cpp/ROOT/main)

ROOT, but not the [particle physics one](https://github.com/root-project/root). Project submission for MATH-458: Programming concepts in scientific computing.

## Project structure

Given that project is relatively simple to structure and does not require installation or packaging, we use the recommended [Canonical Project Structure](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html) for C++ projects.

```
.
├── .clang-format             # ClangFormat configuration
├── .clang-tidy               # ClangTidy configuration
├── .github                   # GitHub related files
│   └── workflows             # CI files to build/test/check the project
├── .gitignore                # To ignore files from git
├── .pre-commit-config.yaml   # Pre-commit configuration
├── CMakeLists.txt            # Top-level CMakeLists.txt
├── Doxyfile                  # Doxygen configuration
├── external                  # External libraries (as git submodules)
│   └── googletest            # Googletest submodule to run tests
├── libROOT                   # Developer-facing ROOT library
│   ├── CMakeLists.txt        # CMakeLists.txt for developer-facing ROOT library
│   ├── libROOT               # CXX and HXX files
│   └── tests                 # Tests for the ROOT library
├── README.md                 # This file
└── ROOT                      # User-facing root_cli application/executable
    ├── CMakeLists.txt        # CMakeLists.txt for user-facing root_cli application/executable
    ├── ROOT                  # CXX and HXX files
    └── tests                 # Tests for the root_cli application/executable
```

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

The library (`libROOT`) and the application (`root_cli`) can be installed using:

```
cmake -S. -DCMAKE_INSTALL_PREFIX=<installation_directory_such_as_/usr/> -Bbuild
cmake --build build --target install
```

Which will put the library, header files, and application in `<install_path>/lib`, `<install_path>/include`,  and `<install_path>/bin` respectively.

## Usage

The installed CLI application can simply be used by:

```
$ <install_path>/bin/root_cli
# or just root_cli if installed in /usr/bin/ on unix for instance
```

And the shared library can be used inside `cxx` files using:

```
# set LD_LIBRARY_PATH to fetch code at runtime
export LD_LIBRARY_PATH=$PWD/<install_path>/lib:$LD_LIBRARY_PATH
# pass the path of headers + link with libROOT + pass the path of the library
g++ <file>.cpp -o <executable_name> -I<install_path>/include -L<install_path>/lib -lROOT
```

## Tests

Tests for the library (`libROOT`) can be found in `libROOT/tests`, and the tests for the application (`root_cli`) can be found in `ROOT/tests`. We follow [test-driven development (TDD)](https://en.wikipedia.org/wiki/Test-driven_development), which means that each code changing PR must have tests to validate the changes. We use `googletest` `v1.17.0` (as a git submodule) to test our code.

All the following tests and checks run as part of a Continuous Integration pipeline on GitHub Actions (on every PR and push to `main`).

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
