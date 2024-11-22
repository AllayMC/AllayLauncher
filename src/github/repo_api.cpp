#include <nlohmann/json.hpp>

#include "github/repo_api.h"

using json = nlohmann::json;

namespace allay_launcher::github {

release_list_t RepoApi::get_releases() const {
    auto url = build_url() + "/releases";

    auto session = create_session();
    session->SetUrl(url);
    auto response = session->Get();

    if (response.status_code == 404) throw NothingException();
    if (response.status_code != 200) {
        throw ConnectionException(
            (int)response.error.code,
            response.status_code,
            response.error.message,
            response.url.str()
        );
    }

    auto ret = release_list_t{};

    json data;

    try {
        data = json::parse(response.text);
    } catch (const json::exception& e) {
        throw JsonException(e.what(), response.text);
    }

    for (const auto& release_data : data) {
        ret.emplace_back(release_t::from_json(release_data));
    }

    return ret;
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

    if (response.status_code == 404) throw NothingException();
    if (response.status_code != 200) {
        throw ConnectionException(
            (int)response.error.code,
            response.status_code,
            response.error.message,
            response.url.str()
        );
    }

    json data;

    try {
        data = json::parse(response.text);
    } catch (const json::exception& e) {
        throw JsonException(e.what(), response.text);
    }

    return release_t::from_json(data);
}

} // namespace allay_launcher::github
