/**
* \file serialize_cmd.hpp
 * \brief Command for serializing multiple input lines into one escaped output line.
 */

#pragma once
#include "command/command.hpp"
#include "options/options.hpp"

/**
 * \class SerializeCmd
 * \brief Command that serializes multiple input lines into a single escaped output line.
 *
 * Reads each line from the input stream as a separate field, applies escaping as needed,
 * and joins them using the configured delimiter.
 */
class SerializeCmd : public ICommand {
    SerializeOptions opts_;  ///< Options specific to serialization mode.

public:
    /**
 * \brief Construct a serialization command.
 * \param opts User-selected serialization options.
 */
    explicit SerializeCmd(const SerializeOptions& opts)
      : opts_(opts) {}

    /**
     * \brief Execute serialization from input to output.
     *
     * Reads all lines from \c in, and serializes them using the configured delimiter
     * and escaping strategy.
     *
     * \param in  Input stream (lines are treated as individual fields).
     * \param out Output stream to write the serialized result.
     * \return 0 on success, 1 on configuration or logic error.
     */
    int execute(std::istream& in, std::ostream& out) override;
};
