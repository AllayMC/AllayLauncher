#include "allay_server.h"

#include "util/file.h"
#include "util/java.h"
#include "util/network.h"
#include "util/os.h"

#include "github/repo_api.h"
#include <filesystem>

namespace allay_launcher {

void AllayServer::run() {
    if (!_check_java()) return;

    util::os::system(
        fmt::format("java -jar {} {}", m_vm_extra_arguments, util::file::read_file(".current_allay_jar_name"))
    );
}

std::expected<void, UpdateAllayError> AllayServer::update(bool use_nightly) {

    github::RepoApi api("https://api.github.com");

    // clang-format off

    api.author("AllayMC")
         .repo("Allay");

    auto release = !use_nightly
         ? api.get_latest_release()
         : api.get_release_by_tag("nightly");

    // clang-format on

    if (!release) {
        logging::error("Something went wrong.");
        logging::error("{}", github::to_string(release.error()));
        return std::unexpected(UpdateAllayError::TODOError);
    }

    auto& assets = release->get_assets();

    if (assets.size() != 1) {
        logging::error("Oh no.");
        return std::unexpected(UpdateAllayError::TODOError);
    }

    auto& asset = assets.front();

    std::string new_allay_jar_name = asset.m_name;
    if (!new_allay_jar_name.starts_with("allay")) {
        // This should never happen in most cases
        logging::error("Asset name should starts with 'allay'.");
        return std::unexpected(UpdateAllayError::TODOError);
    }

    auto current_allay_jar_name = util::file::read_file(".current_allay_jar_name");
    if (new_allay_jar_name == current_allay_jar_name) {
        logging::info("Your allay version is up to date!");
        return std::unexpected(UpdateAllayError::TODOError);
    }
    logging::info("New version {} found! Starting to update.", new_allay_jar_name);

    auto tmp_file_name = new_allay_jar_name + ".tmp";
    // That may because the last time user try to update
    // allay but interrupt the process. We should remove
    // the old tmp file as we do not know if it is completed
    allay_launcher::util::file::remove_if_exists(tmp_file_name);

    auto result = util::network::download(asset.m_browser_download_url, tmp_file_name);
    if (!result) {
        logging::error(error_util::to_string(result.error()));
        std::filesystem::remove(tmp_file_name);
        return std::unexpected(UpdateAllayError::TODOError);
    }
    allay_launcher::util::file::remove_if_exists(new_allay_jar_name);
    std::filesystem::rename(tmp_file_name, new_allay_jar_name);

    // Write the new allay jar name after making sure the file is downloaded properly
    util::file::clean_and_write_file(".current_allay_jar_name", new_allay_jar_name);
    logging::info("Successfully updated to {}.", new_allay_jar_name);

    return {};
}

bool AllayServer::_check_java() {
    bool is_java_ok = false;

    auto version = util::java::installed_version();
    if (version) {
        Version min_required_version{21, 0, 0};
        if (*version < min_required_version) {
            logging::error("Unsupported java version: {}", *version);
            logging::error("Please update your java to 21 or higher.");
        } else {
            is_java_ok = true;
        }
    } else {
        logging::error("Failed to check java version, please make sure if java is installed correctly.");
    }

    if (!is_java_ok) {
        logging::error("Check https://docs.allaymc.org/getting-started/installation/#install-java");
        return false;
    }

    logging::info("Detected java version: {}", format(fg(fmt::color::green), "{}", *version));
    return is_java_ok;
}

} // namespace allay_launcher