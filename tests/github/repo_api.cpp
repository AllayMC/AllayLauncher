#include "test_suite.h" // IWYU pragma: keep

#include "github/repo_api.h"

using namespace github;

static std::unique_ptr<RepoApi> api;

TEST(TestGithubRepoApi, Construct) {
    api = std::make_unique<RepoApi>("https://api.github.com");
    api->author("AllayMC");
    api->repo("AllayLauncher");
}

TEST(TestGithubRepoApi, GetReleases) { EXPECT_GT(api->get_releases().size(), 0); }
TEST(TestGithubRepoApi, GetReleaseById) { EXPECT_NO_THROW(api->get_release_by_id("187454314")); }
TEST(TestGithubRepoApi, GetReleaseByTag) { EXPECT_NO_THROW(api->get_release_by_tag("v1.0.0-BETA")); }
TEST(TestGithubRepoApi, GetLatestRelease) { EXPECT_NO_THROW(api->get_latest_release()); }

TEST(TestGithubRepoApi, Assets) {
    auto as = api->get_release_by_tag("v1.0.0-BETA").get_assets();
    EXPECT_EQ(as.front().m_name, "allay-launcher-linux-x64-release");
    EXPECT_EQ(
        as.front().m_browser_download_url,
        "https://github.com/AllayMC/AllayLauncher/releases/download/v1.0.0-BETA/allay-launcher-linux-x64-release"
    );
}
