/**
* \file deserialize_cmd.hpp
 * \brief Command for deserializing a single escaped input line into multiple fields.
 */

#pragma once
#include "command/icommand.hpp"
#include "options/options.hpp"

/**
 * \class DeserializeCmd
 * \brief Command that deserializes one line of input into individual fields.
 *
 * This command reads one escaped, serialized line from input and outputs
 * each field on a separate line to output.
 */
class DeserializeCmd : public ICommand {
    DeserializeOptions opts_; ///< Options specific to deserialization mode.

public:
    /**
 * \brief Construct a deserialization command.
 * \param opts User-selected deserialization options.
 */
    explicit DeserializeCmd(const DeserializeOptions &opts);

    /**
     * \brief Execute deserialization from input to output.
     *
     * Reads one line from \c in, deserializes it using the configured delimiter,
     * and prints each field on a separate line.
     *
     * \param in  Input stream to read the serialized line from.
     * \param out Output stream to write deserialized fields.
     * \return 0 on success, 1 on parsing error.
     */
    int execute(std::istream &in, std::ostream &out) override;
};
