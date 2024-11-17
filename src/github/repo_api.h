#pragma once

#include "github/internal/api_base.h"
#include "github/internal/release.h"

#include "github/internal/error.h"

namespace allay_launcher::github {

class RepoApi : public ApiBase {
public:
    using ApiBase::ApiBase;

    RepoApi& author(std::string_view author) {
        m_author = author;
        return *this;
    }

    RepoApi& repo(std::string_view repo) {
        m_repo = repo;
        return *this;
    }

    std::expected<release_list_t, GetReleaseError> get_releases() const;

    std::expected<release_t, GetReleaseError> get_release_by_id(std::string_view id) const;
    std::expected<release_t, GetReleaseError> get_release_by_tag(std::string_view tag) const;
    std::expected<release_t, GetReleaseError> get_latest_release() const;

protected:
    cpr::Url build_url() const override { return ApiBase::build_url() + "/repos/" + m_author + "/" + m_repo; }

private:
    std::string m_author;
    std::string m_repo;

    std::expected<release_t, GetReleaseError> _fetch_release(cpr::Url url) const;
};

} // namespace allay_launcher::github