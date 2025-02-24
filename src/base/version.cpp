#include "version.h"

#include "util/string.h"

namespace allay_launcher {

std::optional<Version> Version::parse(std::string_view str) {
    // 0 -> major
    // 1 -> minor
    // 2 -> revision

    auto extract = [&str]() {
        std::array<int32_t, 3> res;
        auto                   draft = util::string::split_nocopy(str, ".");
        for (int i = 0; i < draft.size(); i++) {
            if (i >= res.size()) break;
            if (auto integer = util::string::to_int32(draft.at(i))) {
                res.at(i) = *integer;
            }
        }
        return res;
    };

    auto draft = extract();
    if (draft.at(0) == 0) {
        return {};
    }

    return Version{draft.at(0), draft.at(1), draft.at(2)};
}

} // namespace allay_launcher