/**
 * \file deserialize_cmd.cpp
 * \brief Implementation of DeserializeCmd.
 */

#include "command/deserialize_cmd.hpp"
#include "serdes/serdes.hpp"
#include <iostream>
#include <string>

void DeserializeCmd::configure(CLI::App &sub) {
    sub.add_option("-d,--delim", delim_, "Field delimiter character (default ',')")
        ->type_name("CHAR")
        ->check(CLI::IsMember(std::vector<char>{',', '|', ';', '\t'}));
    sub.add_option("-e,--escape", escape_char_, "Escape character (default '\\')")
        ->type_name("CHAR");
}

auto DeserializeCmd::execute(std::istream &in, std::ostream &out) -> int {
    try {
        serdes::validate_params(delim_, escape_char_);
    } catch (const std::exception &ex) {
        std::cerr << "error: " << ex.what() << "\n";
        return 1;
    }

    std::string line;
    if (!std::getline(in, line)) {
        // Nothing to process
        return 0;
    }

    std::vector<std::string> fields;
    try {
        fields = serdes::deserialize(line, delim_, escape_char_);
    } catch (const std::exception &ex) {
        std::cerr << "error: " << ex.what() << "\n";
        return 1;
    }

    for (auto &f : fields) {
        out << f << "\n";
    }
    return 0;
}
