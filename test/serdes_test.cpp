#include <gtest/gtest.h>
#include <serdes/serdes.hpp>

TEST(SerializeTest, EscapesSpecialCharacters) {
    std::vector<std::string> fields = {"a", "b,c", "d\\e", "f\ng", "h\ri"};
    std::string expected = "a,b\\,c,d\\\\e,f\\ng,h\\ri\n";
    EXPECT_EQ(serdes::serialize(fields, ','), expected);
}

TEST(DeserializeTest, ReversesEscaping) {
    std::string line = "a,b\\,c,d\\\\e,f\\ng,h\\ri";
    std::vector<std::string> expected = {"a", "b,c", "d\\e", "f\ng", "h\ri"};
    EXPECT_EQ(serdes::deserialize(line, ','), expected);
}