#include "serdes/serdes.hpp"
#include <gtest/gtest.h>

TEST(DeserializeTest, ReversesEscaping) {
    const std::string line = R"(a,b\,c,d\\e,f\ng,h\ri)";
    const std::vector<std::string> expected = {"a", "b,c", "d\\e", "f\ng", "h\ri"};
    EXPECT_EQ(serdes::deserialize(line, ','), expected);
}

TEST(DeserializeTest, TrailingEscapeThrows) {
    EXPECT_THROW(serdes::deserialize("foo\\", ','), std::invalid_argument);
}

TEST(DeserializeTest, InvalidEscapeSequenceThrows) {
    EXPECT_THROW(serdes::deserialize("foo\\xc,bar", ','), std::invalid_argument);
}

TEST(DeserializeTest, EmptyInputGivesOneEmptyField) {
    const auto res = serdes::deserialize("", ',');
    ASSERT_EQ(res.size(), 1u);
    EXPECT_EQ(res[0], "");
}

TEST(SerializeTest, MultipleEmptyFields) {
    const std::vector<std::string> fields = {"", ""};
    EXPECT_EQ(serdes::serialize(fields, ','), ",\n");
}

TEST(SerializeTest, EscapeAndDelimiterCharacters) {
    const std::vector<std::string> fields = {"\\", ","};
    EXPECT_EQ(serdes::serialize(fields, ','), "\\\\,\\,\n");
}

TEST(SerializeTest, MixedCRLF) {
    const std::vector<std::string> fields = {"line1\nline2\rline3"};
    EXPECT_EQ(serdes::serialize(fields, ','), "line1\\nline2\\rline3\n");
}

TEST(SerializeTest, LeadingAndTrailingDelimitersInField) {
    const std::vector<std::string> fields = {",start", "end,"};
    EXPECT_EQ(serdes::serialize(fields, ','), "\\,start,end\\,\n");
}