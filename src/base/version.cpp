#include "version.h"

#include "util/string.h"

namespace allay_launcher {

std::expected<Version, SimpleError> Version::parse(std::string_view str) {
    auto tokens = util::string::split(str, ".");
    if (tokens.size() < 3) {
        return std::unexpected(SimpleError::Failed);
    }

    auto major    = util::string::to_int32(tokens.at(0));
    auto minor    = util::string::to_int32(tokens.at(1));
    auto revision = util::string::to_int32(tokens.at(2));

    if (!major || !minor || !revision) {
        return std::unexpected(SimpleError::Failed);
    }

    return Version{*major, *minor, *revision};
}

} // namespace allay_launcher