#include "allay_server.h"

#include "spdlog/spdlog.h"
#include "util/file.h"
#include "util/network.h"
#include "util/os.h"
#include "util/string.h"

#include "github/internal/error.h"
#include "github/repo_api.h"


namespace allay_launcher {

bool AllayServer::run() {
    auto filename = util::file::read_file(".current_allay_jar_name");
    if (!std::filesystem::exists(filename)) {
        logging::error("Jar file not found. Please run 'allay' without any argument to get the jar file.");
        std::filesystem::remove(".current_allay_jar_name");
        return false;
    }
    auto cmd = fmt::format("java -jar {} {}", m_vm_extra_arguments, filename);
    logging::info("Used command: {}", cmd);
    util::os::system(cmd);
    return true;
}

void AllayServer::update(bool use_nightly) {
    using namespace github;

    RepoApi api("https://api.github.com");

    // clang-format off

    api.author("AllayMC")
         .repo("Allay");
    
    release_t release;

    try {
        release = !use_nightly
             ? api.get_latest_release()
             : api.get_release_by_tag("nightly");
    } catch(const GetReleaseException& e) {
        logging::error(e.what());
        throw UpdateAllayException::GetReleaseError();
    }

    // clang-format on

    auto& assets = release.get_assets();

    if (assets.size() != 1) {
        logging::error("Wrong assert count which should be one.");
        throw UpdateAllayException::WrongAssertCount();
    }

    auto& asset = assets.front();

    std::string new_allay_jar_name = asset.m_name;
    if (!util::string::starts_with(new_allay_jar_name, "allay")) {
        // This should never happen in most cases
        logging::error("Asset name should starts with 'allay'.");
        throw UpdateAllayException::WrongFileName();
    }

    auto current_allay_jar_name = util::file::read_file(".current_allay_jar_name");
    if (new_allay_jar_name == current_allay_jar_name) {
        logging::info("Your allay version is up to date!");
        return;
    }
    logging::info("New version {} found! Starting to update.", new_allay_jar_name);

    auto tmp_file_name = new_allay_jar_name + ".tmp";
    // That may because the last time user try to update
    // allay but interrupt the process. We should remove
    // the old tmp file as we do not know if it is completed
    allay_launcher::util::file::remove_if_exists(tmp_file_name);

    try {
        util::network::download(*api.create_session(), asset.m_browser_download_url, tmp_file_name);
    } catch (const DownloadFileException& e) {
        logging::error(e.what());
        throw UpdateAllayException::DownloadFileError();
    }
    allay_launcher::util::file::remove_if_exists(new_allay_jar_name);
    std::filesystem::rename(tmp_file_name, new_allay_jar_name);

    // Write the new allay jar name after making sure the file is downloaded properly
    util::file::clean_and_write_file(".current_allay_jar_name", new_allay_jar_name);
    logging::info("Successfully updated to {}.", new_allay_jar_name);
    // Delete the old jar file
    std::filesystem::remove(current_allay_jar_name);

    return;
}

} // namespace allay_launcher
