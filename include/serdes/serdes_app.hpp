/**
* \file serdes_app.hpp
 * \brief Entry point class for the serialization/deserialization logic.
 *
 * This class encapsulates the runtime execution of the application after
 * CLI parsing. It dispatches to the appropriate command based on user options.
 */

#pragma once
#include "command/icommand.hpp"
#include "options/options.hpp"
#include <memory>

/**
 * \class SerDesApp
 * \brief Application-level controller that selects and runs the appropriate command.
 *
 * Based on the parsed options, this class instantiates either a serialization or
 * deserialization command and executes it against standard input/output streams.
 */
class SerDesApp {
public:
    /**
 * \brief Construct the application logic based on user options.
 * \param opts Immutable options struct produced by CLI parsing.
 * \throws std::logic_error if options contain unknown mode.
 */
    explicit SerDesApp(const Options& opts);

    /**
 * \brief Run the selected command.
 * \return Exit code: 0 on success, non-zero on error.
 */
    int run();

private:
    /// The command to execute (serialization or deserialization).
    std::unique_ptr<ICommand> cmd_;
};