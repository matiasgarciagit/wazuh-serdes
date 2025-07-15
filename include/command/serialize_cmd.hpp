/**
 * \file serialize_cmd.hpp
 * \brief Command for serializing multiple input lines into one escaped output line.
 */

#pragma once
#include "command/icommand.hpp"
#include "options/options.hpp"

/**
 * \class SerializeCmd
 * \brief Command that serializes multiple input lines into a single escaped output line.
 *
 * Reads each line from the input stream as a separate field, applies escaping as needed,
 * and joins them using the configured delimiter.
 */
class SerializeCmd : public ICommand {
  public:
    SerializeCmd() = default;

    /**
     * \brief Declare serialization-specific flags/options.
     * \param sub  CLI11 subcommand to attach options to.
     *
     * Options:
     *  - -d, --delim CHAR     Field delimiter character (default ',')
     *  - -e, --escape CHAR    Escape character (default '\\')
     *  -     --no-escape      Disable escaping entirely
     */
    void configure(CLI::App &sub) override;

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
    auto execute(std::istream &in, std::ostream &out) -> int override;

  private:
    char delim_ = ',';        ///< Character used to delimit fields.
    char escape_char_ = '\\'; ///< Character used to escape special chars.
    bool no_escape_ = false;  ///< If true, do not apply any escaping.
};
