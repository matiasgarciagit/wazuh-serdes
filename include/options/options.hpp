//
// Created by mgarcia on 07/07/25.
//

#pragma once

#include <variant>

struct SerializeOptions {
    char  delim           = ',';    // separator
    bool  disable_escape  = false;  // not escape anything
};

struct DeserializeOptions {
    char delim = ',';               // separator
};

enum class Mode { Serialize, Deserialize };

struct Options {
    Mode                       mode;
    std::variant<
      SerializeOptions,
      DeserializeOptions
    > opts;
};