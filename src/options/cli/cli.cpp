/**
 * \file cli.cpp
 * \brief Implementation of the CLI parser using CLI11.
 *
 * Configures global flags, user options, and the
 * `serialize` / `deserialize` subcommands. Handles
 * parsing errors by printing help or version info
 * and exiting with the appropriate code.
 */

#include "options/cli/cli.hpp"
#include "app/version.hpp"
#include <CLI/CLI.hpp>
#include <CLI/Error.hpp>
#include <cstdlib>

namespace Cli {

static constexpr auto kProgramName = app::info::name;
static constexpr auto kProgramVer = app::info::version;

// anonymous namespace = internal linkage for implementation details
namespace {

/**
 * \brief Validator helper enforcing exactly one UTF-8 code unit.
 *
 * Used for the `-d,--delim` option to ensure the user
 * provides exactly one printable ASCII character.
 *
 * \return A `CLI::Validator` that checks `s.size() == 1`.
 */
CLI::Validator one_char_validator() {
    return {[](const std::string &s) {
                return s.size() == 1 ? std::string{}
                                     : std::string{"delimiter must be a single ASCII character"};
            },
            "CHAR", "char_len_1"};
}

} // namespace

auto parse(const int argc, char **argv) -> Options {
    CLI::App app;
    // description + version
    app.description(std::string(kProgramName) +
                    " — serialize/deserialize text fields with reversible escaping (v" +
                    std::string(kProgramVer) + ")");

    // Flags
    app.set_help_flag("-h,--help", "Show this help message and exit")->group("FLAGS");
    app.set_version_flag("-v,-V,--version", static_cast<std::string>(kProgramVer),
                         "Show program version and exit")
        ->group("FLAGS");

    // Global options
    char delim = ',';
    bool disable_escape = false;
    app.add_option("-d,--delim", delim, "Field delimiter character (default ',')")
        ->type_name("CHAR")
        ->check(one_char_validator())
        ->group("OPTIONS");
    app.add_flag("--no-escape", disable_escape,
                 "Disable escaping (not reversible; only works with 'serialize'; "
                 "useful for debugging or benchmarking)")
        ->group("OPTIONS");

    // Usage y groups
    app.usage([&]() {
        std::ostringstream oss;
        oss << "Usage:\n"
            << "  " << kProgramName << " [FLAGS] [OPTIONS] [SUBCOMMANDS]";
        return oss.str();
    });
    auto fmt = app.get_formatter();
    fmt->label("Flags", "FLAGS:");
    fmt->label("Options", "OPTIONS:");
    fmt->label("Subcommands", "SUBCOMMANDS:");

    // add_subcommands
    const auto *cmd_ser = app.add_subcommand(
        "serialize", "Read lines from stdin and output one escaped line to stdout");
    const auto *cmd_des = app.add_subcommand(
        "deserialize", "Read one escaped line from stdin and output one field per line");

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
        return Options{Mode::Serialize, SerializeOptions{delim, disable_escape}};
    } else {
        return Options{Mode::Deserialize, DeserializeOptions{delim}};
    }
}

} // namespace Cli