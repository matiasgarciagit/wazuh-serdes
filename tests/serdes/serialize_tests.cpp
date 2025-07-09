#include "serdes/serdes.hpp"
#include <stdexcept>
#include <gtest/gtest.h>

TEST(SerializeTest, EscapesSpecialCharacters) {
    std::vector<std::string> fields = {"a", "b,c", "d\\e", "f\ng", "h\ri"};
    const std::string expected = "a,b\\,c,d\\\\e,f\\ng,h\\ri\n";
    EXPECT_EQ(serdes::serialize(fields, ','), expected);
}

TEST(SerializeTest, EmptyFieldsVectorProducesEmptyString) {
    const std::vector<std::string> empty;
    EXPECT_EQ(serdes::serialize(empty, ','), "");
}

TEST(SerializeTest, SingleEmptyFieldProducesJustNewline) {
    const std::vector<std::string> v{""};
    EXPECT_EQ(serdes::serialize(v, ','), "\n");
}

TEST(SerializeTest, InvalidParamsThrows) {
    EXPECT_NO_THROW(serdes::serialize({"foo"}, ',', '\\'));
    EXPECT_THROW(serdes::serialize({"foo"}, '\\', '\\'),
                 std::invalid_argument);
    EXPECT_THROW(serdes::serialize({"foo"}, '\n', '\\'),
                 std::invalid_argument);
    EXPECT_THROW(serdes::serialize({"foo"}, '\r', '\\'),
                 std::invalid_argument);
    EXPECT_THROW(serdes::serialize({"foo"}, '\x01', '\\'),
                 std::invalid_argument);
}

TEST(DeserializeTest, ConsecutiveDelimiters) {
    const std::string line = "a,,b";
    const std::vector<std::string> expected = {"a", "", "b"};
    EXPECT_EQ(serdes::deserialize(line, ','), expected);
}

TEST(DeserializeTest, TrailingDelimiter) {
    const std::string line = "a,b,";
    std::vector<std::string> expected = {"a", "b", ""};
    EXPECT_EQ(serdes::deserialize(line, ','), expected);
}

TEST(DeserializeTest, OnlyDelimiter) {
    const std::string line = ",";
    const std::vector<std::string> expected = {"", ""};
    EXPECT_EQ(serdes::deserialize(line, ','), expected);
}

TEST(DeserializeTest, OnlyEscapeSequence) {
    const std::string line = "\\n";
    const std::vector<std::string> expected = {"\n"};
    EXPECT_EQ(serdes::deserialize(line, ','), expected);
}

TEST(DeserializeTest, MixedCRLF) {
    const std::string line = "line1\\nline2\\rline3";
    const std::vector<std::string> expected = {"line1\nline2\rline3"};
    EXPECT_EQ(serdes::deserialize(line, ','), expected);
}

TEST(DeserializeTest, ComplexField) {
    const std::string line = "\\\\\\,\\\\n\\\\r\\\\";
    const std::vector<std::string> expected = {"\\,\\n\\r\\"};
    EXPECT_EQ(serdes::deserialize(line, ','), expected);
}