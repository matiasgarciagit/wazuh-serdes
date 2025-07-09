#include <gtest/gtest.h>

#include <serdes/serdes.hpp>
#include <command/serialize_cmd.hpp>
#include <command/deserialize_cmd.hpp>

#include <sstream>
#include <vector>
#include <string>


TEST(Integration, CoreRoundTripVector) {
    const std::vector<std::string> original = {
        "simple",
        "with,comma",
        "with\\backslash",
        "with\nnewline",
        "with\rcarriage"
    };

    const std::string serialized = serdes::serialize(original, ',', '\\');
    const auto round = serdes::deserialize(
        serialized.back()=='\n' ? serialized.substr(0, serialized.size()-1)
                                : serialized,
        ',', '\\'
    );

    EXPECT_EQ(round, original);
}

TEST(Integration, CommandRoundTrip) {
    SerializeOptions so;
    so.delim           = ';';
    so.disable_escape  = false;
    SerializeCmd sCmd(so);

    std::istringstream inStream(
        "field1\n"
        "fi;eld2\n"
        "fi\\eld3\n"
    );
    std::ostringstream midStream;
    ASSERT_EQ(sCmd.execute(inStream, midStream), 0);

    const std::string serialized = midStream.str();

    DeserializeOptions dOpts;
    dOpts.delim = ';';
    DeserializeCmd dCmd(dOpts);

    std::istringstream in2(serialized);
    std::ostringstream outStream;
    ASSERT_EQ(dCmd.execute(in2, outStream), 0);

    std::string expected =
        "field1\n"
        "fi;eld2\n"
        "fi\\eld3\n";
    EXPECT_EQ(outStream.str(), expected);
}
