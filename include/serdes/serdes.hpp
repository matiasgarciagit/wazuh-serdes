/**
 * \file serdes.hpp
 * \brief Core serialization and deserialization logic (header).
 *
 * All escaping and unescaping follows a reversible scheme that handles
 * delimiters, newlines, carriage returns, and backslashes.
 */

#pragma once
#include <string>
#include <vector>

namespace serdes {

/**
 * \brief Serializes a list of fields into a single escaped line.
 *
 * Escapes special characters so that the output can later be deserialized
 * without loss. Appends a newline if the input is non-empty.
 *
 * \param fields List of input fields.
 * \param delim Delimiter character.
 * \param escape Escape character.
 * \return Escaped single-line string representing the fields.
 * \throws std::invalid_argument If `delim == escape` or invalid characters used.
 */
auto serialize(const std::vector<std::string> &fields, char delim = ',', char escape = '\\')
    -> std::string;

/**
 * \brief Deserializes a single escaped line into multiple fields.
 *
 * Reverses the escaping done by `serialize()`. Every escape sequence
 * must be valid; otherwise, an error is thrown.
 *
 * \param input Serialized line without trailing newline.
 * \param delim Delimiter character used during serialization.
 * \param escape Escape character used during serialization.
 * \return List of individual fields.
 * \throws std::invalid_argument On invalid escape sequence or bad parameters.
 */
auto deserialize(const std::string &input, char delim = ',', char escape = '\\')
    -> std::vector<std::string>;

/**
 * \brief Validate delimiter and escape characters for use in serialization.
 *
 * Ensures they are printable, distinct, and not line-breaking characters.
 *
 * \param delim Field delimiter.
 * \param escape Escape character.
 * \throws std::invalid_argument On invalid delimiter or escape.
 */
void validate_params(char delim, char escape);
} // namespace serdes
