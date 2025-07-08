#include <iostream>

#include "options/cli/cli.hpp"
#include "serdes/serdes_app.hpp"

int main(const int argc, char** argv)
{
    try {
        const auto opts = Cli::parse(argc, argv);
        return SerDesApp{opts}.run();
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
}