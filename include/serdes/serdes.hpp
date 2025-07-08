//
// Created by mgarcia on 07/07/25.
//

#pragma once
#include <string>
#include <vector>

namespace serdes {
    /// @brief Serializes a list of text fields into a single escaped line.
    /// @param fields    Vector of input fields (each may contain any characters).
    /// @param delim     Delimiter character to separate fields.
    /// @param escape    Escape character (must not be the same as delim).
    /// @returns         A single string with all fields joined and escaped, ending in '\n' if fields non-empty.
    /// @throws std::invalid_argument if delim == escape.
    std::string serialize(
        const std::vector<std::string>& fields,
        char delim = ',',
        char escape = '\\'
    );

    /// @brief Deserializes one escaped line into a list of text fields.
    /// @param input    The serialized line (without the trailing '\n').
    /// @param delim    Field delimiter (same one that was used to serialize).
    /// @param escape   Escape character (same one that was used to serialize).
    /// @returns        Vector of fields, with all escape sequences decoded.
    /// @throws std::invalid_argument on bad delimiter/escape or invalid escape seq.
    std::vector<std::string> deserialize(
        const std::string& input,
        char delim = ',',
        char escape = '\\'
    );

    /// Validate that delim & escape are sensible (printable, different, no nl/cr).
    /// @throws std::invalid_argument on bad parameters
    void validate_params(char delim, char escape);
}
