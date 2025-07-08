//
// Created by mgarcia on 07/07/25.
//

#pragma once
#include <ostream>

class ICommand {
public:
    virtual ~ICommand() = default;
    /// Exec cmd; return exit code
    virtual int execute(std::istream& in, std::ostream& out) = 0;
};