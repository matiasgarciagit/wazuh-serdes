/**
 * \file serdes.cpp
 * \brief Implementation of serialization/deserialization functions.
 */

#include "serdes/serdes.hpp"
#include <cctype>
#include <stdexcept>

namespace serdes {
void validate_params(char delim, char escape) {
    if (delim == escape) {
        throw std::invalid_argument("serialize/deserialize: delimiter and escape must differ");
    }
    if (delim == '\n' || delim == '\r') {
        throw std::invalid_argument("serialize/deserialize: delimiter must not be '\\n' or '\\r'");
    }
    if (!std::isprint(static_cast<unsigned char>(delim))) {
        throw std::invalid_argument("serialize/deserialize: delimiter must be printable ASCII");
    }
}

auto serialize(const std::vector<std::string> &fields, char delim, char escape) -> std::string {
    validate_params(delim, escape);

    // Reserve worst-case: each byte → 2, plus one delimiter per field
    size_t est = 0;
    for (auto &f : fields)
        est += f.size() * 2 + 1;
    std::string out;
    out.reserve(est);

    bool first = true;
    for (const auto &field : fields) {
        if (!first)
            out.push_back(delim);
        first = false;

        for (unsigned char uc : field) {
            if (uc == static_cast<unsigned char>(escape)) {
                out.push_back(escape);
                out.push_back(escape);
            } else if (uc == static_cast<unsigned char>(delim)) {
                out.push_back(escape);
                out.push_back(delim);
            } else if (uc == '\n') {
                out.push_back(escape);
                out.push_back('n');
            } else if (uc == '\r') {
                out.push_back(escape);
                out.push_back('r');
            } else {
                out.push_back(static_cast<char>(uc));
            }
        }
    }

    if (!fields.empty())
        out.push_back('\n');
    return out;
}

auto deserialize(const std::string &input, char delim, char escape) -> std::vector<std::string> {
    validate_params(delim, escape);

    std::vector<std::string> fields;
    std::string cur;
    cur.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        if (const auto uc = static_cast<unsigned char>(input[i]);
            uc == static_cast<unsigned char>(escape)) {
            // Must have a next character
            if (i + 1 >= input.size()) {
                throw std::invalid_argument("deserialize: trailing escape");
            }
            if (const auto nxt = static_cast<unsigned char>(input[++i]);
                nxt == static_cast<unsigned char>(escape)) {
                cur.push_back(escape);
            } else if (nxt == static_cast<unsigned char>(delim)) {
                cur.push_back(delim);
            } else if (nxt == 'n') {
                cur.push_back('\n');
            } else if (nxt == 'r') {
                cur.push_back('\r');
            } else {
                throw std::invalid_argument(std::string("deserialize: invalid escape sequence \\") +
                                            static_cast<char>(nxt));
            }
        } else if (uc == static_cast<unsigned char>(delim)) {
            // Field boundary
            fields.push_back(std::move(cur));
            cur.clear();
            cur.reserve(input.size()); // avoid repeated realloc
        } else {
            cur.push_back(static_cast<char>(uc));
        }
    }

    // push last field (even if empty)
    fields.push_back(std::move(cur));
    return fields;
}
} // namespace serdes
