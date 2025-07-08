/**
* \file deserialize_cmd.cpp
 * \brief Implementation of DeserializeCmd.
 */

#include "command/deserialize_cmd.hpp"
#include "serdes/serdes.hpp"
#include <string>
#include <iostream>

DeserializeCmd::DeserializeCmd(const DeserializeOptions &opts): opts_(opts) {}

int DeserializeCmd::execute(std::istream& in, std::ostream& out) {
    // Read exactly one line of serialized input
    std::string line;
    if (!std::getline(in, line)) {
        // Nothing to process
        return 0;
    }

    try {
        const auto fields = serdes::deserialize(line, opts_.delim, '\\');
        for (auto const& f : fields) {
            out << f << '\n';
        }
    } catch (const std::exception& e) {
        out << "error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
