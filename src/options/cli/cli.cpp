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

Options parse(const int argc, char **argv)
{
    CLI::App app{
        std::string{kProgramName} +
        " — serialize/deserialize text fields with reversible escaping"};

    /* ── global flags ──────────────────────────────────────────────── */
    bool show_version{false};
    app.set_help_flag("-h,--help", "Show this help message and exit");
    app.add_flag("-v,-V,--version", show_version,
                 "Show program version and exit")
       ->group("Generic options");

    /* ── serialize sub-commands ──────────────────────────────────────────────── */
    auto* cmd_ser = app.add_subcommand("serialize", "Emit one línea serializada");
    char  ser_delim           = ',';
    bool  ser_disable_escape  = false;
    cmd_ser->add_option("-d,--delim", ser_delim,
                        "Delimiter character")
          ->check(one_char_validator());
    cmd_ser->add_flag("--no-escape", ser_disable_escape,
                      "Disable escaping (raw mode)");

    /* ── deserialize sub-commands ──────────────────────────────────────────────── */
    auto* cmd_des = app.add_subcommand("deserialize", "Emit un campo por línea");
    char des_delim = ',';
    cmd_des->add_option("-d,--delim", des_delim,
                        "Delimiter character")
           ->check(one_char_validator());


    /* ── parse argv  ────────────────────────────────────────────── */
    try {
        app.parse(argc, argv);                 // may throw ParseError / CallForHelp
    }
    catch (const CLI::ParseError &e) {
        std::exit(app.exit(e, std::cout, std::cerr));  // prints help or error
    }

    /* ── --version early exit ──────────────────────────────────── */
    if (show_version) {
        std::cout << kProgramName << ' ' << kProgramVer << '\n';
        std::exit(EXIT_SUCCESS);
    }

    /* ── no sub-command?  behave as --help ─────────────────────── */
    if (!*cmd_ser && !*cmd_des) {
        std::exit(app.exit(CLI::CallForHelp{}, std::cout, std::cerr));
    }

    // map chosen sub-command
    if (*cmd_ser) {
        SerializeOptions so;
        so.delim          = ser_delim;
        so.disable_escape = ser_disable_escape;
        return Options{ Mode::Serialize, so };
    } else {
        DeserializeOptions dopt;
        dopt.delim = des_delim;
        return Options{ Mode::Deserialize, dopt };
    }
}

}