#include <nlohmann/json.hpp>

#include "release.h"

using json = nlohmann::json;

namespace allay_launcher::github {

std::optional<Release> Release::from_json(const nlohmann::json& data) {
    Release ret;
    try {
        for (const auto& asset_data : data["assets"]) {
            auto asset = release_t::asset_t::from_json(asset_data);
            if (!asset) return {};
            ret.add_asset(*asset);
        }
    } catch (const json::exception& e) {
        return {};
    }
    return ret;
}

std::optional<Release::Asset> Release::Asset::from_json(const nlohmann::json& data) {
    try {
        Release::Asset asset;
        asset.m_name                 = data["name"];
        asset.m_browser_download_url = data["browser_download_url"];
        return asset;
    } catch (const json::exception& e) {
        return {};
    }
}

} // namespace allay_launcher::github