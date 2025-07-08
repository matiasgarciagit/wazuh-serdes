//
// Created by mgarcia on 07/07/25.
//

#pragma once
#include "command/command.hpp"
#include "options/options.hpp"

class DeserializeCmd : public ICommand {
    DeserializeOptions opts_;

public:
    explicit DeserializeCmd(const DeserializeOptions& opts)
      : opts_(opts) {}

    /// @brief Read one serialized line, deserialize it, and write one field per line.
    /// @returns exit code (0 = success, 1 = error)
    int execute(std::istream& in, std::ostream& out) override;
};