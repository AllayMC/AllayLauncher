#pragma once

#include "spdlog/fmt/ostr.h"

namespace allay_launcher {

struct Version {
    static std::expected<Version, SimpleError> parse(std::string_view str);

    auto operator<=>(const Version&) const = default;

    operator std::string() const { return fmt::format("{}.{}.{}", m_major, m_minor, m_revision); }

    int m_major;
    int m_minor;
    int m_revision;
};

} // namespace allay_launcher

constexpr std::ostream& operator<<(std::ostream& os, const allay_launcher::Version& c) {
    return os << static_cast<std::string>(c);
}

template <>
struct fmt::formatter<allay_launcher::Version> : fmt::ostream_formatter {};
