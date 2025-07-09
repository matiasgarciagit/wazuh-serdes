/**
* \file options.hpp
 * \brief Command-line options model.
 *
 * This file defines the runtime configuration model used to drive
 * serialization or deserialization logic.
 */

#pragma once
#include <variant>

/**
 * \brief Options for serialization mode.
 */
struct SerializeOptions {
    char  delim           = ',';    // separator
    bool  disable_escape  = false;  // not escape anything
};

/**
 * \brief Options for deserialization mode.
 */
struct DeserializeOptions {
    char delim = ',';               // separator
};

/**
 * \brief Application mode selected by the user.
 */
enum class Mode { Serialize, Deserialize };

/**
 * \brief Top-level options structure returned by CLI parsing.
 *
 * Wraps the selected mode and associated mode-specific configuration.
 */
struct Options {
    Mode                       mode; ///< Operation mode.
    std::variant<
      SerializeOptions,
      DeserializeOptions
    > opts;                         ///< Mode-specific options.
};