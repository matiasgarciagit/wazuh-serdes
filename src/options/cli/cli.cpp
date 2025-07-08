//
// Created by mgarcia on 07/07/25.
//

#include "options/cli/cli.hpp"
#include <CLI/CLI.hpp>
#include <CLI/Error.hpp>
#include <cstdlib>

namespace Cli {

constexpr std::string_view kProgramName   = "wazuh-serdes";
constexpr std::string_view kProgramVer    = "0.1.0";

/// Local helper: validator that enforces *exactly* one UTF-8 code unit.
static CLI::Validator one_char_validator() {
    return {
        [](const std::string &s) {
            return s.size() == 1
                       ? std::string{}
                       : std::string{"delimiter must be a single ASCII character"};
        },
        "CHAR", "char_len_1"
    };
}

    Options parse(const int argc, char** argv) {
    CLI::App app;
    // description + version
    app.description(
        std::string(kProgramName) +
        " — serialize/deserialize text fields with reversible escaping (v" +
        std::string(kProgramVer) + ")"
    );

    // Flags
    app.set_help_flag("-h,--help", "Show this help message and exit")
       ->group("FLAGS");
    app.set_version_flag("-v,-V,--version", static_cast<std::string>(kProgramVer),
        "Show program version and exit")
       ->group("FLAGS");

    // Global options
    char delim = ',';
    bool disable_escape = false;
    app.add_option("-d,--delim", delim,
        "Field delimiter character (default ',')")
       ->type_name("CHAR")
       ->check(one_char_validator())
       ->group("OPTIONS");
    app.add_flag("--no-escape", disable_escape,
        "Disable escaping (not reversible; only works with 'serialize'; "
        "useful for debugging or benchmarking)")
       ->group("OPTIONS");

    // Usage y groups
    app.usage([&](){
        std::ostringstream oss;
        oss << "Usage:\n"
            << "  " << kProgramName << " [FLAGS] [OPTIONS] [SUBCOMMANDS]";
        return oss.str();
    });
    auto fmt = app.get_formatter();
    fmt->label("Flags",      "FLAGS:");
    fmt->label("Options",    "OPTIONS:");
    fmt->label("Subcommands","SUBCOMMANDS:");

    // add_subcommands
    const auto* cmd_ser = app.add_subcommand("serialize",
        "Read lines from stdin and output one escaped line to stdout");
    const auto* cmd_des = app.add_subcommand("deserialize",
        "Read one escaped line from stdin and output one field per line");

    // Parse
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        std::exit(app.exit(e));
    }

    if (!*cmd_ser && !*cmd_des) {
        std::exit(app.exit(CLI::CallForHelp{}, std::cout, std::cerr));
    }

    // Map result
    if (*cmd_ser) {
        return Options{ Mode::Serialize,
            SerializeOptions{delim, disable_escape} };
    } else {
        return Options{ Mode::Deserialize,
            DeserializeOptions{delim} };
    }
}

}