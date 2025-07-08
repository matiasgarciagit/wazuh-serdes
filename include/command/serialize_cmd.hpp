//
// Created by mgarcia on 07/07/25.
//

#pragma once
#include "command/command.hpp"
#include "options/options.hpp"

class SerializeCmd : public ICommand {
    SerializeOptions opts_;

public:
    explicit SerializeCmd(const SerializeOptions& opts)
      : opts_(opts) {}

    /// @brief Read all lines, serialize them, and write to output.
    /// @returns exit code (0 = success, 1 = bad configuration)
    int execute(std::istream& in, std::ostream& out) override;
};
