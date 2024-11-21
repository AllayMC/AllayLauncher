#include <nlohmann/json.hpp>

#include "github/repo_api.h"

using json = nlohmann::json;

namespace allay_launcher::github {

release_list_t RepoApi::get_releases() const {
    auto session = create_session();

    session->SetUrl(build_url() + "/releases");
    auto response = session->Get();

    if (response.status_code == 404) throw GetReleaseError::NotFound();
    if (response.status_code != 200) {
        logging::error("Unable to get release list. Status code: {}", response.status_code);
        throw GetReleaseException::NetworkError();
    }

    try {
        auto ret  = release_list_t{};
        auto data = json::parse(response.text);
        for (const auto& release_data : data) {
            if (auto release = release_t::from_json(release_data)) {
                ret.emplace_back(*release);
            } else {
                throw GetReleaseException::JsonError();
            }
        }
        return ret;
    } catch (const json::exception& e) {
        throw GetReleaseException::JsonError();
    }
}

release_t RepoApi::get_release_by_id(std::string_view id) const {
    return _fetch_release(build_url() + "/releases/" + id.data());
}

release_t RepoApi::get_release_by_tag(std::string_view tag) const {
    return _fetch_release(build_url() + "/releases/tags/" + tag.data());
}

release_t RepoApi::get_latest_release() const { return _fetch_release(build_url() + "/releases/latest"); }

release_t RepoApi::_fetch_release(const cpr::Url& url) const {
    auto session = create_session();

    session->SetUrl(url);
    auto response = session->Get();

    if (response.status_code == 404) throw GetReleaseException::NotFound();
    if (response.status_code != 200) {
        logging::error("Unable to fetch release. Status code: {}", response.status_code);
        throw GetReleaseException::NetworkError();
    }

    try {
        auto data = json::parse(response.text);
        if (auto release = release_t::from_json(data)) {
            return *release;
        } else {
            throw GetReleaseException::JsonError();
        }
    } catch (const json::exception& e) {
        throw GetReleaseException::JsonError();
    }
}

} // namespace allay_launcher::github