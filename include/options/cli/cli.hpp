//
// Created by mgarcia on 07/07/25.
//

#pragma once
#include "options/options.hpp"

namespace Cli {

    /// Parse `argc/argv` and return an immutable Options struct.
    /// Throws std::exception on bad input (let main() catch & log).
    Options parse(int argc, char** argv);

}
