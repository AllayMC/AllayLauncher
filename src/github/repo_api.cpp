#include <nlohmann/json.hpp>

#include "github/repo_api.h"

using json = nlohmann::json;

namespace allay_launcher::github {

std::expected<release_list_t, GetReleaseError> RepoApi::get_releases() const {
    auto session = create_session();

    session.SetUrl(build_url() + "/releases");
    auto response = session.Get();

    if (response.status_code == 404) return std::unexpected(GetReleaseError::NotFound);
    if (response.status_code != 200) return std::unexpected(GetReleaseError::NetworkError);

    try {
        auto ret  = release_list_t{};
        auto data = json::parse(response.text);
        for (const auto& release_data : data) {
            if (auto release = release_t::from_json(release_data)) {
                ret.emplace_back(*release);
            } else {
                return std::unexpected(GetReleaseError::JsonError);
            }
        }
        return ret;
    } catch (const json::exception& e) {
        return std::unexpected(GetReleaseError::JsonError);
    }
}

std::expected<release_t, GetReleaseError> RepoApi::get_release_by_id(std::string_view id) const {
    return _fetch_release(build_url() + "/releases/" + id.data());
}

std::expected<release_t, GetReleaseError> RepoApi::get_release_by_tag(std::string_view tag) const {
    return _fetch_release(build_url() + "/releases/tags/" + tag.data());
}

std::expected<release_t, GetReleaseError> RepoApi::get_latest_release() const {
    return _fetch_release(build_url() + "/releases/latest");
}

std::expected<release_t, GetReleaseError> RepoApi::_fetch_release(cpr::Url url) const {
    auto session = create_session();

    session.SetUrl(url);
    auto response = session.Get();

    if (response.status_code == 404) return std::unexpected(GetReleaseError::NotFound);
    if (response.status_code != 200) return std::unexpected(GetReleaseError::NetworkError);

    try {
        auto data = json::parse(response.text);
        if (auto release = release_t::from_json(data)) {
            return *release;
        } else {
            return std::unexpected(GetReleaseError::JsonError);
        }
    } catch (const json::exception& e) {
        return std::unexpected(GetReleaseError::JsonError);
    }
}

} // namespace allay_launcher::github