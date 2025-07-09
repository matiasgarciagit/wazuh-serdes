/**
* \file cli.hpp
 * \brief Declaration of the command-line parsing module (CLI).
 *
 * Defines the public interface to parse `argc`/`argv` and produce
 * the options structure (`Options`) that drives the application.
 */

#pragma once
#include "options/options.hpp"

namespace Cli {

    /**
     * \brief Parses command-line arguments.
     *
     * Interprets flags, options and subcommands
     * (`serialize` / `deserialize`), validating syntax
     * and building a ready-to-use `Options` object.
     *
     * \param argc Argument count in `argv`.
     * \param argv Argument vector.
     * \return An `Options` struct containing the selected mode
     *         and its parameters.
     * \throws std::exception If the syntax is invalid
     *         (e.g. unknown flag, malformed delimiter).
     */
    Options parse(int argc, char** argv);

}
