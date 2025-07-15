/**
 * \file main.cpp
 * \brief Entry-point for the SerDes utility.
 *
 * This file wires together command-line parsing and the core
 * serializer/deserializer application object (`SerDesApp`).
 */

#include "command/deserialize_cmd.hpp"
#include "command/serialize_cmd.hpp"
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
    SerDesApp app;

    // instantiate commands with default options
    // (default delimiter = ',', default escape_char = '\\')
    SerializeCmd serializeCmd{};
    DeserializeCmd deserializeCmd{};

    struct CommandEntry {
        std::string_view name;
        std::string_view description;
        ICommand &command;
    };

    const std::array<CommandEntry, 2> commands{
        {{.name = "serialize",
          .description = "Read lines from stdin and write a single serialized line",
          .command = serializeCmd},
         {.name = "deserialize",
          .description = "Read a serialized line from stdin and write one field per line",
          .command = deserializeCmd}}};

    for (auto const &[name, description, command] : commands) {
        app.register_command(name, description, command);
    }

    return app.run(argc, argv);
}