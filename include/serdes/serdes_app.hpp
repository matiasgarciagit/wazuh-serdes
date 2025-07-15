/**
 * \file serdes_app.hpp
 * \brief Entry point class for the serialization/deserialization logic.
 *
 * This class encapsulates the runtime execution of the application after
 * CLI parsing. It dispatches to the appropriate command based on user options.
 */

#pragma once

#include "command/icommand.hpp"
#include <CLI/CLI.hpp>
#include <string_view>

/**
 * \class SerDesApp
 * \brief Application-level controller for registering and executing commands.
 *
 * SerDesApp holds a CLI11 parser, lets you register any number of
 * ICommand-backed subcommands, and dispatches to the chosen one
 * after parsing argc/argv.
 */
class SerDesApp {
  public:
    /**
     * \brief Construct the application parser.
     * \param name        Program name shown in help (default "wazuh-serdes").
     * \param description Short description shown in help.
     */
    explicit SerDesApp(std::string_view name = "wazuh-serdes",
                       std::string_view description = "Serialize and deserialize text fields");

    /**
     * \brief Register a new subcommand.
     * \param name        Subcommand name (e.g. "serialize").
     * \param description Help text for this subcommand.
     * \param command     Reference to an ICommand implementation.
     *
     * This will create a CLI11 subcommand, call command.configure(sub),
     * and hook sub->callback() to command.execute().
     */
    void register_command(std::string_view name, std::string_view description, ICommand &command);

    /**
     * \brief Parse command-line arguments and execute the selected subcommand.
     * \param argc Argument count from main().
     * \param argv Argument vector from main().
     * \return Exit code: 0 on success, non-zero on error.
     */
    auto run(int argc, char **argv) -> int;

  private:
    CLI::App app_; ///< Underlying CLI11 app for parsing and help
};