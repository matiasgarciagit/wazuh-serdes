/**
* \file serdes_app.cpp
 * \brief Implementation of the SerDesApp class.
 */

#include "serdes/serdes_app.hpp"
#include "command/serialize_cmd.hpp"
#include "command/deserialize_cmd.hpp"

#include <iostream>


SerDesApp::SerDesApp(const Options& opts) {
    switch (opts.mode) {
        case Mode::Serialize: {
            // extract SerializeOptions from variant
            const auto& serOpts = std::get<SerializeOptions>(opts.opts);
            cmd_ = std::make_unique<SerializeCmd>(serOpts);
            break;
        }
        case Mode::Deserialize: {
            //extract DeserializeOptions from variant
            const auto& desOpts = std::get<DeserializeOptions>(opts.opts);
            cmd_ = std::make_unique<DeserializeCmd>(desOpts);
            break;
        }
        default:
            throw std::logic_error("Modo desconocido en SerDesApp");
    }
}

int SerDesApp::run() {
    return cmd_->execute(std::cin, std::cout);
}
