#include <gtest/gtest.h>
#include <serdes/serdes.hpp>

TEST(ValidateParamsTest, ThrowsIfDelimEqualsEscape) {
    EXPECT_THROW(serdes::validate_params(',', ','), std::invalid_argument);
}

TEST(ValidateParamsTest, ThrowsIfNonPrintableDelimiter) {
    EXPECT_THROW(serdes::validate_params('\n', '\\'), std::invalid_argument);
    EXPECT_THROW(serdes::validate_params('\r', '\\'), std::invalid_argument);
    EXPECT_THROW(serdes::validate_params('\x01', '\\'), std::invalid_argument);
}

TEST(ValidateParamsTest, AcceptsValidParams) {
    EXPECT_NO_THROW(serdes::validate_params(',', '\\'));
    EXPECT_NO_THROW(serdes::validate_params(';', '\\'));
}
