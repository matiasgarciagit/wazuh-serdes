# wazuh-serdes

A small C++20 command-line utility to **serialize** and **deserialize** text fields with reversible escaping.  
Supports two modes:

- **serialize**: read multiple lines (fields) from stdin, join them into a single escaped line on stdout.
- **deserialize**: read one escaped line from stdin, print each field on its own line.

Delimiters (default `,`) and escaping (`\`) are fully configurable so that commas, newlines and backslashes can appear in your data without breaking the format. A round-trip (`deserialize(serialize(fields))`) yields exactly the original input.

---

## Table of Contents

1. [Features](#features)
2. [Prerequisites](#prerequisites)
3. [Building](#building)
4. [Usage](#usage)
5. [Examples](#examples)
6. [Testing](#testing)
7. [Documentation](#documentation)
8. [3rd Party Software Used](#3rd-party-software-used)
9. [License](#license)

---

## Features

-  **Two modes**: `serialize` / `deserialize` subcommands
-  **Configurable delimiter** (`-d, --delim`, default `,`)
-  **Optional escaping** disable for raw joins (`--no-escape`)
-  **Handles** delimiter, newline (`\n`), carriage return (`\r`) and backslash (`\`) via reversible escape sequences
-  **Round-trip safe**: deserializing a serialized line yields the original fields

---

## Prerequisites

- A C++20-capable compiler (GCC 11+, Clang 11+, MSVC 2019+)
- [CMake](https://cmake.org/) ≥ 3.14
- Internet access (to fetch [CLI11](https://github.com/CLIUtils/CLI11) and [GoogleTest](https://github.com/google/googletest))
- (Optional) [Doxygen](https://www.doxygen.nl/) if you want to generate the HTML documentation

---

## Building

Build with Ninja (or your preferred generator):

1. Open a terminal and `cd` into the **root** of the unzipped project:
   ```bash
   cd /path/to/wazuh-serdes

2. Create a build directory and enter it:
    ```bash
    mkdir build
    cd build

3. Configure and generate with CMake (Debug or Release):
    ```bash
    cmake -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      ..

4. Compile:
    ```bash
   ninja

5. You’ll now have:
    ```bash
   ./wazuh-serdes — the CLI tool

   ./tests/serdes-tests — unit + integration tests for the core

   ./tests/command-tests — tests for the command-layer

   ./tests/cli-tests — tests for the CLI parser

---

## Usage

Run `wazuh-serdes` with one of two subcommands:

```bash
  wazuh-serdes [FLAGS] [OPTIONS] <SUBCOMMAND>
```

- **Flags**:
    - `-h, --help`  
      Show help message and exit.
    - `-v, -V, --version`  
      Show program version and exit.

- **Options**:
    - `-d, --delim CHAR`  
      Field delimiter character (default: `,`).
    - `--no-escape`  
      Disable escaping (serialize-only; not reversible; useful for debugging or benchmarking).

- **Subcommands**:
    - `serialize`  
      Read lines from stdin (each line is a field) and output one escaped line to stdout.
    - `deserialize`  
      Read one escaped line from stdin and output each field on its own line.

---

## Examples

### 1. Serializing simple fields

```bash
  printf "apple\nbanana\ncherry" | ./wazuh-serdes serialize
# → apple,banana,cherry
```

### 2. Escaping a comma in a field

```bash
  printf "foo,bar\nbaz" | ./wazuh-serdes serialize
# → foo\,bar,baz
```

### 3. Deserializing back to lines

```bash
  echo "foo\,bar,baz" | ./wazuh-serdes deserialize
# → foo,bar
#   baz
```

### 4. Using a custom delimiter (before the subcommand)

```bash
  printf "one|two|three" | ./wazuh-serdes -d '|' serialize
# → one\|two\|three

  echo "one|two|three" | ./wazuh-serdes -d '|' deserialize
# → one
#   two
#   three
```

### 5. Disabling escape (before the subcommand)

```bash
  printf "a,b\nc" | ./wazuh-serdes --no-escape serialize
# → a,b,c
```

### 6. Full round-trip

```bash
  printf '%s\n' "line1" "line,2" "line\3" "line" "4" | \
  ./wazuh-serdes -d '|' serialize | \
  ./wazuh-serdes -d '|' deserialize
# → line1
#   line,2
#   line\3
#   line
#   4
```
---

## Testing

After building, your test binaries live in the `build/tests/` directory. You can execute them in two ways:

### 1. Run all tests via CTest

```bash
  cd build
  ctest --output-on-failure
  # or, if you're using Ninja:
  ninja test
  # (or with Makefiles: make test)
```

### 2. Run individual test suites

```bash
  ./tests/serdes-tests
  ./tests/command-tests
  ./tests/cli-tests
```

### Sample output for `serdes-tests`

```bash
$ ./tests/serdes-tests
  Running main() from /home/user/wazuh-serdes/cmake-build-debug/_deps/googletest-src/googletest/src/gtest_main.cc
  [==========] Running 23 tests from 4 test suites.
  [----------] 10 tests from DeserializeTest
  [ RUN      ] DeserializeTest.ReversesEscaping
  [       OK ] DeserializeTest.ReversesEscaping (0 ms)
      
  [----------] 3 tests from ValidateParamsTest
  [ RUN      ] ValidateParamsTest.AcceptsValidParams
  [       OK ] ValidateParamsTest.AcceptsValidParams (0 ms)
  [----------] 3 tests from ValidateParamsTest (0 ms total)
  [----------] Global test environment tear-down
  [==========] 23 tests from 4 test suites ran. (0 ms total)
  [  PASSED  ] 23 tests.

```
---

## Documentation

All API reference and design docs are generated with [Doxygen](https://www.doxygen.nl/) using the `Doxyfile` in the project root.

- **Manual** generation:

  ```bash
  doxygen Doxyfile
  ```

- **Via CMake** (requires the `docs` target):

  ```bash
  cmake --build build --target docs
  ```

The HTML output lands in the `docs/` directory at the project root. Open:

```
docs/index.html
```

in your browser to browse the generated documentation.

---

## 3rd Party Software Used

| Software    | Description                        | License       | Version |
|-------------|------------------------------------|---------------|---------|
| CLI11       | Command-line option parser for C++ | MIT           | v2.5.0  |
| GoogleTest  | Unit testing framework for C++     | BSD 3-Clause  | v1.17.0 |

---

## License

**Project License:** MIT License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for the full text.