#include <gtest/gtest.h>
#include <command/serialize_cmd.hpp>
#include <sstream>

TEST(SerializeCmdTest, DefaultBehavior) {
    SerializeOptions opts;
    opts.delim = ',';
    opts.disable_escape = false;
    SerializeCmd cmd(opts);

    std::istringstream in("foo\nbar,baz\n");
    std::ostringstream out;
    int code = cmd.execute(in, out);

    EXPECT_EQ(code, 0);
    EXPECT_EQ(out.str(), "foo,bar\\,baz\n");
}

TEST(SerializeCmdTest, DisableEscapeRawJoin) {
    SerializeOptions opts;
    opts.delim = ';';
    opts.disable_escape = true;
    SerializeCmd cmd(opts);

    std::istringstream in("one\ntwo;three\n");
    std::ostringstream out;
    EXPECT_EQ(cmd.execute(in, out), 0);
    EXPECT_EQ(out.str(), "one;two;three\n");
}

TEST(SerializeCmdTest, ValidateErrorReturnsNonZero) {
    // delim == escape => validate_params throws
    SerializeOptions opts;
    opts.delim = '\\';
    opts.disable_escape = false;
    SerializeCmd cmd(opts);

    std::istringstream in("x\ny\n");
    std::ostringstream out;
    int code = cmd.execute(in, out);

    EXPECT_EQ(code, 1);
    EXPECT_TRUE(out.str().rfind("error:", 0) == 0);
}
