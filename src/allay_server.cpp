#include "allay_server.h"

#include "util/file.h"
#include "util/network.h"
#include "util/os.h"
#include "util/string.h"

#include "github/repo_api.h"

constexpr std::string_view ALLAY_SERVER_JAR_NAME = ".allay_current";
constexpr std::string_view ALLAY_NIGHTLY_TAG     = "nightly";

namespace allay_launcher {

bool AllayServer::run() {
    logging::info("Launching server...");

    auto jar_name = _current_jar_name();
    if (jar_name.empty() || !std::filesystem::exists(jar_name)) {
        logging::error("Allay server not found. Please run 'allay' without any argument to get the jar file.");
        return false;
    }
    auto cmd = fmt::format("java -jar {} {}", m_vm_extra_arguments, jar_name);
    util::os::run(cmd);
    return true;
}

void AllayServer::update(bool use_nightly) {
    using namespace github;

    RepoApi api("https://api.github.com");

    // clang-format off

    api.author("AllayMC")
         .repo("Allay");
    
    release_t release = !use_nightly
        ? api.get_latest_release()
        : api.get_release_by_tag(ALLAY_NIGHTLY_TAG);

    // clang-format on

    auto& assets = release.get_assets();
    if (assets.size() != 1) throw NeedUpdateException("assets.size() != 1");

    auto& asset = assets.front();
    if (!util::string::starts_with(asset.m_name, "allay"))
        throw NeedUpdateException("asset.name does not start with 'allay'");

    std::string new_jar_name = asset.m_name;

    std::string guessed_version = new_jar_name;
    util::string::remove_prefix(guessed_version, "allay-server-");
    util::string::remove_suffix(guessed_version, "-shaded.jar");

    // If the file names are the same, then an update is assumed to exist.
    auto old_jar_name = _current_jar_name();

    if (new_jar_name == old_jar_name) {
        logging::info("Currently using the latest {}version!", use_nightly ? "nightly " : "");
        return;
    }

    logging::info("New version {} found! Starting to update...", format(fmt::emphasis::bold, guessed_version));

    util::network::download(asset.m_browser_download_url, new_jar_name);

    if (auto digest = asset.m_digest; util::string::remove_prefix(digest, "sha256:")) {
        if (util::file::calc_sha256(new_jar_name) != digest) {
            throw ChecksumException("the file may have been corrupted during transfer", new_jar_name);
        }
    } else {
        throw ChecksumException("please report an issue", new_jar_name);
    }

    // Write the new allay jar name after making sure the file is downloaded properly.
    _current_jar_name(new_jar_name);
    logging::info("Successfully updated to {}.", format(fmt::emphasis::bold, guessed_version));

    // Delete the old jar file.
    std::filesystem::remove(old_jar_name);
}

std::string AllayServer::_current_jar_name() const {
    std::ifstream ifs(ALLAY_SERVER_JAR_NAME.data());
    return ifs.is_open() ? std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>())
                         : std::string{};
}

void AllayServer::_current_jar_name(std::string_view new_current_jar_name) {
    if (std::ofstream ofs(ALLAY_SERVER_JAR_NAME.data(), std::ios::trunc); ofs) {
        ofs << new_current_jar_name;
    }
}

} // namespace allay_launcher
