//
// Created by mgarcia on 07/07/25.
//

#pragma once
#include "command/command.hpp"
#include "options/options.hpp"
#include <memory>

class SerDesApp {
public:
    explicit SerDesApp(const Options& opts);
    int run();

private:
    std::unique_ptr<ICommand> cmd_;
};