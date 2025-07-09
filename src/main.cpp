/**
 * \file main.cpp
 * \brief Entry-point for the SerDes utility.
 *
 * This file wires together command-line parsing and the core
 * serializer/deserializer application object (`SerDesApp`).
 */

#include "options/cli/cli.hpp"
#include "serdes/serdes_app.hpp"
#include <iostream>

/**
 * \brief Program entry-point.
 *
 * Parses CLI options and launches the application.  All
 * user-visible errors are printed to `stderr`; any exception
 * escaping `SerDesApp::run()` terminates the program with
 * an exit code of 1.
 *
 * \param argc Argument count.
 * \param argv Argument vector.
 * \return 0 on success, non-zero on failure.
 */
auto main(const int argc, char **argv) -> int {
    try {
        const auto opts = Cli::parse(argc, argv);
        return SerDesApp{opts}.run();
    } catch (const std::exception &e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
}