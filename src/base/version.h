#pragma once

#include "spdlog/fmt/ostr.h"

namespace allay_launcher {

struct Version {
    static std::optional<Version> parse(std::string_view str);

    // C++20
    // auto operator<=>(const Version&) const = default;

    bool operator<(const Version& other) const {
        return std::tie(m_major, m_minor, m_revision) < std::tie(other.m_major, other.m_minor, other.m_revision);
    }

    bool operator>(const Version& other) const { return other < *this; }

    bool operator<=(const Version& other) const { return !(other < *this); }

    bool operator>=(const Version& other) const { return !(*this < other); }

    bool operator==(const Version& other) const {
        return std::tie(m_major, m_minor, m_revision) == std::tie(other.m_major, other.m_minor, other.m_revision);
    }

    bool operator!=(const Version& other) const { return !(*this == other); }

    // to string

    operator std::string() const { return fmt::format("{}.{}.{}", m_major, m_minor, m_revision); }

    friend std::ostream& operator<<(std::ostream& os, const allay_launcher::Version& c) {
        return os << static_cast<std::string>(c);
    }

    int m_major;
    int m_minor;
    int m_revision;
};

} // namespace allay_launcher

template <>
struct fmt::formatter<allay_launcher::Version> : fmt::ostream_formatter {};
