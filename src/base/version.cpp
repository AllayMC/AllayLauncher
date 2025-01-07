#include "version.h"

#include "util/string.h"

namespace allay_launcher {

std::optional<Version> Version::parse(std::string_view str) {
    auto tokens = util::string::split(str, ".");
    if (tokens.size() > 3) {
        return {};
    }

    // major(require),minor,revision
    std::vector<std::optional<int32_t>> version = {0, 0, 0};

    auto vId = 0;
    for (const auto& item : tokens) {
        version[vId] = util::string::to_int32(tokens[vId]);
        vId++;
    }

    if (!version[0]) return {};
    auto ret = Version{*version[0], 0, 0};
    if (version[1]) ret.m_minor = *version[1];
    if (version[2]) ret.m_revision = *version[2];
    return ret;
}

} // namespace allay_launcher