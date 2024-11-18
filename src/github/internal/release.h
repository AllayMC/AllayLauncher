#pragma once

#include <nlohmann/json_fwd.hpp>

namespace allay_launcher::github {

class Release {
public:
    static std::expected<Release, SimpleError> from_json(const nlohmann::json& data);

    struct Asset {
        static std::expected<Asset, SimpleError> from_json(const nlohmann::json& data);

        std::string m_name;
        std::string m_browser_download_url;
    };

    using asset_t      = Asset;
    using asset_list_t = std::vector<asset_t>;

    void                add_asset(const Asset& asset) { m_assets.emplace_back(asset); }
    asset_list_t const& get_assets() const { return m_assets; }

private:
    std::vector<Asset> m_assets;
};

using release_t       = Release;
using release_asset_t = Release::Asset;

using release_list_t = std::vector<release_t>;

} // namespace allay_launcher::github