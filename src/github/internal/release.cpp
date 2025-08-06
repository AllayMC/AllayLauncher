#include <nlohmann/json.hpp>

#include "github/internal/release.h"

using json = nlohmann::json;

namespace allay_launcher::github {

Release Release::from_json(const nlohmann::json& data) try {
    Release ret;
    for (const auto& asset_data : data["assets"]) {
        ret.add_asset(release_t::asset_t::from_json(asset_data));
    }
    return ret;
} catch (const json::exception& e) {
    throw JsonException(e.what(), data.dump());
}

Release::Asset Release::Asset::from_json(const nlohmann::json& data) try {
    Release::Asset asset;
    asset.m_name                 = data["name"];
    asset.m_browser_download_url = data["browser_download_url"];
    asset.m_digest               = data["digest"];
    return asset;
} catch (const json::exception& e) {
    throw JsonException(e.what(), data.dump());
}

} // namespace allay_launcher::github