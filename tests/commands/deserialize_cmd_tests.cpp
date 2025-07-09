#include <gtest/gtest.h>
#include <command/deserialize_cmd.hpp>
#include <sstream>

TEST(DeserializeCmdTest, BasicSplit) {
    DeserializeOptions opts;
    opts.delim = ',';
    DeserializeCmd cmd(opts);

    std::istringstream in("a,b,c\n");
    std::ostringstream out;
    EXPECT_EQ(cmd.execute(in, out), 0);
    EXPECT_EQ(out.str(), "a\nb\nc\n");
}

TEST(DeserializeCmdTest, EmptyInputDoesNothing) {
    DeserializeOptions opts;
    opts.delim = ',';
    DeserializeCmd cmd(opts);

    std::istringstream in("");
    std::ostringstream out;
    EXPECT_EQ(cmd.execute(in, out), 0);
    EXPECT_EQ(out.str(), "");
}

TEST(DeserializeCmdTest, InvalidEscapeOutputsError) {
    DeserializeOptions opts;
    opts.delim = ',';
    DeserializeCmd cmd(opts);

    std::istringstream in("bad\\x,\n");
    std::ostringstream out;
    EXPECT_EQ(cmd.execute(in, out), 1);
    EXPECT_NE(out.str().find("error:"), std::string::npos);
}
