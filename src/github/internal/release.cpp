#include "release.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace allay_launcher::github {

std::expected<Release, SimpleError> Release::from_json(const nlohmann::json& data) {
    Release ret;
    try {
        for (const auto& asset_data : data["assets"]) {
            auto asset = release_t::asset_t::from_json(asset_data);
            if (!asset) return std::unexpected(SimpleError::Failed);
            ret.add_asset(*asset);
        }
    } catch (const json::exception& e) {
        return std::unexpected(SimpleError::Failed);
    }
    return ret;
}

std::expected<Release::Asset, SimpleError> Release::Asset::from_json(const nlohmann::json& data) {
    try {
        return Release::Asset{.m_name = data["name"], .m_browser_download_url = data["browser_download_url"]};
    } catch (const json::exception& e) {
        return std::unexpected(SimpleError::Failed);
    }
}

} // namespace allay_launcher::github