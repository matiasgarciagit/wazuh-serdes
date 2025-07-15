/**
 * \file serialize_cmd.cpp
 * \brief Implementation of SerializeCmd.
 */

#include "command/serialize_cmd.hpp"
#include "serdes/serdes.hpp"
#include <CLI/Validators.hpp>
#include <istream>
#include <string>

void SerializeCmd::configure(CLI::App &sub) {
    sub.add_option("-d,--delim", delim_, "Field delimiter character (default ',')")
        ->type_name("CHAR")
        ->check(CLI::IsMember(std::vector<char>{',', '|', ';', '\t'}));
    sub.add_option("-e,--escape", escape_char_, "Escape character (default '\\')")
        ->type_name("CHAR");
    sub.add_flag("--no-escape", no_escape_, "Disable escaping entirely; raw join fields");
}

auto SerializeCmd::execute(std::istream &in, std::ostream &out) -> int {
    // Validate parameters if escaping is enabled
    if (!no_escape_) {
        try {
            serdes::validate_params(delim_, escape_char_);
        } catch (const std::exception &ex) {
            std::cerr << "error: " << ex.what() << "\n";
            return 1;
        }
    }

    // Read all lines into a vector of fields
    std::vector<std::string> fields;
    std::string line;
    while (std::getline(in, line)) {
        fields.push_back(std::move(line));
    }

    // If no fields, just emit an empty line
    if (fields.empty()) {
        out << "\n";
        return 0;
    }

    // Raw join (no escaping)
    if (no_escape_) {
        for (size_t i = 0; i < fields.size(); ++i) {
            if (i)
                out.put(delim_);
            out << fields[i];
        }
        out << "\n";
        return 0;
    }

    // Escaped serialization
    try {
        const auto result = serdes::serialize(fields, delim_, escape_char_);
        out << result << "\n";
    } catch (const std::exception &ex) {
        std::cerr << "error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
