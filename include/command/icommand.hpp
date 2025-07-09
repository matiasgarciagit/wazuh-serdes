/**
* \file icommand.hpp
 * \brief Abstract command interface for serialization/deserialization operations.
 *
 * All commands operate on standard input/output streams and return a process exit code.
 */

#pragma once
#include <ostream>

/**
 * \class ICommand
 * \brief Base interface for all executable commands.
 *
 * This abstraction is used by the application controller to delegate the actual
 * serialization or deserialization logic.
 */
class ICommand {
public:
    virtual ~ICommand() = default;

    /**
     * \brief Execute the command logic.
     *
     * \param in  Input stream to read from (usually std::cin).
     * \param out Output stream to write to (usually std::cout).
     * \return Exit code: 0 = success, non-zero = failure.
     */
    virtual int execute(std::istream& in, std::ostream& out) = 0;
};