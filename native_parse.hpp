#pragma once

#include "doof_runtime.hpp"
#include "types.hpp"

#include <cerrno>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <string>

namespace doof_parse {

inline bool hasOuterWhitespace(const std::string& value) {
    if (value.empty()) return false;
    const auto first = static_cast<unsigned char>(value.front());
    const auto last = static_cast<unsigned char>(value.back());
    return std::isspace(first) || std::isspace(last);
}

template <typename T>
inline doof::Result<T, ParsingError> emptyOrInvalid(const std::string& value) {
    return doof::Failure<ParsingError>{
        value.empty() ? ParsingError::EmptyInput : ParsingError::InvalidFormat
    };
}

inline doof::Result<bool, ParsingError> parseBool(const std::string& value) {
    if (value == "true") return doof::Success<bool>{true};
    if (value == "false") return doof::Success<bool>{false};
    return emptyOrInvalid<bool>(value);
}

template <typename T>
inline doof::Result<T, ParsingError> parseSignedInteger(const std::string& value) {
    if (value.empty() || hasOuterWhitespace(value)) {
        return emptyOrInvalid<T>(value);
    }

    errno = 0;
    char* end = nullptr;
    const long long parsed = std::strtoll(value.c_str(), &end, 10);
    if (end == value.c_str() || (end != nullptr && *end != '\0')) {
        return doof::Failure<ParsingError>{ParsingError::InvalidFormat};
    }
    if (errno == ERANGE) {
        return doof::Failure<ParsingError>{
            value.front() == '-' ? ParsingError::Underflow : ParsingError::Overflow
        };
    }
    if (parsed < static_cast<long long>(std::numeric_limits<T>::min())) {
        return doof::Failure<ParsingError>{ParsingError::Underflow};
    }
    if (parsed > static_cast<long long>(std::numeric_limits<T>::max())) {
        return doof::Failure<ParsingError>{ParsingError::Overflow};
    }
    return doof::Success<T>{static_cast<T>(parsed)};
}

inline doof::Result<uint8_t, ParsingError> parseByte(const std::string& value) {
    return parseSignedInteger<uint8_t>(value);
}

inline doof::Result<int32_t, ParsingError> parseInt(const std::string& value) {
    return parseSignedInteger<int32_t>(value);
}

inline doof::Result<int64_t, ParsingError> parseLong(const std::string& value) {
    return parseSignedInteger<int64_t>(value);
}

template <typename T>
inline doof::Result<T, ParsingError> parseFloating(
    const std::string& value,
    T (*convert)(const char*, char**)
) {
    if (value.empty() || hasOuterWhitespace(value)) {
        return emptyOrInvalid<T>(value);
    }

    errno = 0;
    char* end = nullptr;
    const T parsed = convert(value.c_str(), &end);
    if (end == value.c_str() || (end != nullptr && *end != '\0')) {
        return doof::Failure<ParsingError>{ParsingError::InvalidFormat};
    }
    if (errno == ERANGE) {
        return doof::Failure<ParsingError>{
            parsed == static_cast<T>(0) ? ParsingError::Underflow : ParsingError::Overflow
        };
    }
    return doof::Success<T>{parsed};
}

inline doof::Result<float, ParsingError> parseFloat(const std::string& value) {
    return parseFloating<float>(value, std::strtof);
}

inline doof::Result<double, ParsingError> parseDouble(const std::string& value) {
    return parseFloating<double>(value, std::strtod);
}

} // namespace doof_parse
