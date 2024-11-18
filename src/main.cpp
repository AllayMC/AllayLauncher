#include <argparse/argparse.hpp>

#include <cstdlib>
#include <spdlog/fmt/bundled/color.h>

#include <cpr/cpr.h>
#include <string>

#include "config.h"

#include "spdlog/spdlog.h"
#include "util/file.h"
#include "util/java.h"
#include "util/progress_bar.h"

#include "github/repo_api.h"
#include "util/string.h"

using namespace allay_launcher;
using logging::fmt_lib::color;

void setup_logger() {
#ifdef DEBUG
    logging::set_level(spdlog::level::debug);
#endif
    logging::set_pattern("[%^%l%$] %v");
}

bool check_java() {
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

    logging::info("Detected java version: {}", format(fg(color::green), "{}", *version));
    return is_java_ok;
}

struct Args {
    bool        m_update;
    bool        m_use_nightly;
    bool        m_run;
    std::string m_java_args;
};

Args parse_arguments(int argc, char* argv[]) {
    if (argc == 1) {
        // Return default args if no arg is provided
        return Args{true, true, true, ""};
    }

    using namespace argparse;

    ArgumentParser program("allay", "0.1.0");

    program.add_argument("-u", "--update").help("Update allay").flag();
    program.add_argument("-n", "--nightly").help("Use nightly build").flag();
    program.add_argument("-r", "--run").help("Run allay server").flag();
    program.add_argument("-a", "--args").help("Pass arguments to java").default_value(std::string(""));

    program.parse_args(argc, argv);

    return Args{
        program.get<bool>("--update"),
        program.get<bool>("--nightly"),
        program.get<bool>("--run"),
        program.get<std::string>("--args")
    };
}

bool update_allay(bool use_nightly) {

    github::RepoApi api("https://api.github.com");

    api.author("AllayMC").repo("Allay");

    auto release = !use_nightly ? api.get_latest_release() : api.get_release_by_tag("nightly");

    if (!release) {
        logging::error("Something went wrong.");
        logging::error("{}", github::to_string(release.error()));
        return false;
    }

    auto& assets = release->get_assets();

    if (assets.size() != 1) {
        logging::error("Oh no.");
        return false;
    }

    auto& asset = assets.front();

    std::string new_allay_jar_name = asset.m_name;
    if (!new_allay_jar_name.starts_with("allay")) {
        // This should never happen in most cases
        logging::error("Asset name should starts with 'allay'.");
        return false;
    }

    auto current_allay_jar_name = util::file::read_file(".current_allay_jar_name");
    if (new_allay_jar_name == current_allay_jar_name) {
        logging::info("Your allay version is up to date!");
        return true;
    }
    logging::info("New version {} found! Starting to update.", new_allay_jar_name);
    std::string download_url = asset.m_browser_download_url;
    if (std::filesystem::exists(new_allay_jar_name)) {
        // That may because the last time user try to update
        // allay but interrupt the process. We should remove
        // the old file as we do not know if it is correct
        std::filesystem::remove(new_allay_jar_name);
    }
    std::ofstream output_file(new_allay_jar_name, std::ios::binary);
    if (!output_file) {
        logging::error("Can't create output file.");
        return false;
    }
    progresscpp::ProgressBar progress_bar(100, 70);
    cpr::cpr_off_t           upload_last       = 0;
    auto                     download_response = cpr::Download(
        output_file,
        cpr::Url{download_url},
        cpr::ProgressCallback(
            [&progress_bar](
                cpr::cpr_off_t download_total,
                cpr::cpr_off_t download_now,
                cpr::cpr_off_t upload_total,
                cpr::cpr_off_t upload_now,
                intptr_t       userdata
            ) -> bool {
                progress_bar.set_progress((float)download_now / download_total);
                progress_bar.display(
                    fmt::format("{:.2}/{:.2} MB", download_now / (1024.0 * 1024.0), download_total / (1024.0 * 1024.0))
                );
                return true;
            }
        )
    );
    progress_bar.done(format(fg(color::green), "\u221a"));
    if (download_response.status_code != 200) {
        logging::error("Failed to download {}. Status code: {}.", download_url, download_response.status_code);
        std::filesystem::remove(new_allay_jar_name);
        return false;
    }
    // Write the new allay jar name after making sure the file is downloaded properly
    util::file::clean_and_write_file(".current_allay_jar_name", new_allay_jar_name);
    logging::info("Successfully updated to {}.", new_allay_jar_name);

    return true;
}

void run_allay(const std::string extra_args) {
    auto cmd = "java -jar " + extra_args + " " + util::file::read_file(".current_allay_jar_name");
    logging::info("Used java command: " + cmd);
    system(cmd.c_str());
    logging::info("Server stopped");
}

int main(int argc, char* argv[]) {

    setup_logger();

    logging::info(
        "Allay launcher {} ({}).",
        format(fg(color::green), ALLAY_LAUNCHER_VERSION),
        format(fg(color::yellow), GIT_COMMIT)
    );

    auto args = parse_arguments(argc, argv);

    if (args.m_update) {
        if (!update_allay(args.m_use_nightly)) {
            logging::error("Error while updating allay.");
            return 1;
        }
    }

    if (args.m_run) {
        if (!check_java()) {
            return 1;
        }
        util::string::remove_prefix(args.m_java_args, "\"");
        util::string::remove_suffix(args.m_java_args, "\"");
        run_allay(args.m_java_args);
    }
    return 0;
}