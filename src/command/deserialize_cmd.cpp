/**
 * \file deserialize_cmd.cpp
 * \brief Implementation of DeserializeCmd.
 */

#include "command/deserialize_cmd.hpp"
#include "serdes/serdes.hpp"
#include <iostream>
#include <string>

DeserializeCmd::DeserializeCmd(const DeserializeOptions &opts) : opts_(opts) {}

auto DeserializeCmd::execute(std::istream &in, std::ostream &out) -> int {
    // Read exactly one line of serialized input
    std::string line;
    if (!std::getline(in, line)) {
        return 0;
    }

    try {
        const auto fields = serdes::deserialize(line, opts_.delim, '\\');
        for (const auto &f : fields) {
            out << f << '\n';
        }
    } catch (const std::exception &e) {
        out << "error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
