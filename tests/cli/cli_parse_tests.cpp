#include "options/cli/cli.hpp"
#include <gtest/gtest.h>

static auto make_argv(std::initializer_list<const char *> args) -> std::vector<char *> {
    std::vector<char *> v;
    for (const auto s : args)
        v.push_back(const_cast<char *>(s));
    return v;
}

TEST(CliParseTest, DefaultSerialize) {
    auto argv = make_argv({"prog", "serialize"});
    auto [mode, opts] = Cli::parse(static_cast<int>(argv.size()), argv.data());
    EXPECT_EQ(mode, Mode::Serialize);
    auto [delim, disable_escape] = std::get<SerializeOptions>(opts);
    EXPECT_EQ(delim, ',');
    EXPECT_FALSE(disable_escape);
}

TEST(CliParseTest, CustomDelimAndDisableEscape) {
    auto argv = make_argv({"prog", "-d", ";", "--no-escape", "serialize"});
    auto [mode, opts] = Cli::parse(static_cast<int>(argv.size()), argv.data());
    EXPECT_EQ(mode, Mode::Serialize);
    auto [delim, disable_escape] = std::get<SerializeOptions>(opts);
    EXPECT_EQ(delim, ';');
    EXPECT_TRUE(disable_escape);
}

TEST(CliParseTest, DeserializeMode) {
    auto argv = make_argv({"prog", "deserialize"});
    auto [mode, opts] = Cli::parse(static_cast<int>(argv.size()), argv.data());
    EXPECT_EQ(mode, Mode::Deserialize);
    auto [delim] = std::get<DeserializeOptions>(opts);
    EXPECT_EQ(delim, ',');
}
