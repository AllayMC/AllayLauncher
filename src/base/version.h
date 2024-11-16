#pragma once

namespace allay_launcher {

struct Version {
    static std::expected<Version, SimpleError> parse(std::string_view str);

    auto operator<=>(const Version&) const = default;

    // TODO: specialization of std::formatter
    operator std::string() { return std::format("{}.{}.{}", m_major, m_minor, m_revision); }

    int m_major;
    int m_minor;
    int m_revision;
};

} // namespace allay_launcher
