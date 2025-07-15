// serdes_app.cpp
// Implementation of SerDesApp: registers ICommand-backed subcommands
// and dispatches based on CLI11 parsing.

#include "serdes/serdes_app.hpp"

#include <CLI/CLI.hpp>
#include <cstdlib> // for std::exit
#include <iostream>

SerDesApp::SerDesApp(std::string_view name, std::string_view description)
    : app_(std::string{name}, std::string{description}) {}

void SerDesApp::register_command(const std::string_view name, const std::string_view description,
                                 ICommand &command) {
    // Create a CLI11 subcommand under the main app
    auto *sub = app_.add_subcommand(std::string{name}, std::string{description});

    // Let the command declare its own flags/options
    command.configure(*sub);

    // Hook the subcommand callback: execute and exit with the returned code
    sub->callback([&command]() {
        const int code = command.execute(std::cin, std::cout);
        std::exit(code);
    });
}

auto SerDesApp::run(int argc, char **argv) -> int {
    // Parse argc/argv; if there's a parse error or --help, CLI11 will handle it
    try {
        CLI11_PARSE(app_, argc, argv);
    } catch (const CLI::ParseError &e) {
        // CLI11::_exit() will have already printed messages,
        // so just return its recommended exit code
        return app_.exit(e);
    }

    // If no subcommand was provided, CLI11 will also trigger help/exit.
    // We should never reach here, but return 0 by convention.
    return 0;
}
