/**
 * \file serialize_cmd.cpp
 * \brief Implementation of SerializeCmd.
 */

#include "command/serialize_cmd.hpp"
#include "serdes/serdes.hpp"
#include <istream>
#include <string>

SerializeCmd::SerializeCmd(const SerializeOptions &opts) : opts_(opts) {}

auto SerializeCmd::execute(std::istream &in, std::ostream &out) -> int {

    try {
        serdes::validate_params(opts_.delim, '\\'); // same checks as in serdes::serialize
    } catch (const std::exception &e) {
        out << "error: " << e.what() << '\n';
        return 1;
    }

    // Read each input line as one field
    std::vector<std::string> fields;
    std::string line;

    while (true) {
        std::getline(in, line);
        if (in.fail())
            break; // no more lines (or an error)
        fields.push_back(std::move(line));
    }

    // Disable escaping (raw join)
    if (opts_.disable_escape) {
        if (fields.empty()) {
            return 0;
        }
        for (size_t i = 0; i < fields.size(); ++i) {
            if (i)
                out.put(opts_.delim); // write delimiter
            out << fields[i];
        }
        out.put('\n'); // terminate with newline
        return 0;
    }

    try {
        const auto serialized = serdes::serialize(fields, opts_.delim, '\\');
        out << serialized;
    } catch (const std::exception &e) {
        out << "error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
