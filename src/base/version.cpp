#include "version.h"

#include "util/string.h"

namespace allay_launcher {

std::optional<Version> Version::parse(std::string_view str) {
    auto tokens = util::string::split(str, ".");
    if (tokens.size() < 3) {
        return {};
    }

    auto major    = util::string::to_int32(tokens.at(0));
    auto minor    = util::string::to_int32(tokens.at(1));
    auto revision = util::string::to_int32(tokens.at(2));

    if (!major || !minor || !revision) return {};

    return Version{*major, *minor, *revision};
}

} // namespace allay_launcher