#include <argparse/argparse.hpp>
#include <cpr/cpr.h>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/fmt/bundled/color.h>
#include <string>

#include "config.h"
#include "cpr/api.h"
#include "util/file.h"
#include "util/java.h"
#include "util/progress_bar.h"


using namespace allay_launcher;
using logging::fmt_lib::color;
using nlohmann::json;

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

auto parse_arguments(int argc, char* argv[]) {
    using namespace argparse;

    ArgumentParser program("allay", "0.1.0");

    // clang-format off

    program.add_argument("-u", "--update")
            .help("Update allay")
            .flag();
    program.add_argument("-n", "--nightly")
            .help("Use nightly build")
            .flag()
            .default_value(true);
    program.add_argument("-r", "--run")
            .help("Run allay server")
            .flag();

    program.parse_args(argc, argv);

    struct {
        bool m_update;
        bool m_use_nightly;
        bool m_run;
    } ret {
        // program.get<bool>("--update"), 
        // program.get<bool>("--nightly"),
        // program.get<bool>("--run")
        true, true, true
    };

    // clang-format on

    return ret;
}

bool update_allay(bool use_nightly) {
    std::string   request_url = use_nightly ? "https://api.github.com/repos/AllayMC/Allay/releases/tags/nightly"
                                            : "https://api.github.com/repos/AllayMC/Allay/releases/latest";
    cpr::Response response    = cpr::Get(cpr::Url{request_url});
    if (response.status_code != 200) {
        logging::error("Can't connect to {}. Status code: {}", request_url, response.status_code);
        return false;
    }

    json json = json::parse(response.text);
    if (json["assets"].empty()) {
        // This should never happen in most cases
        logging::error("Asset not found.");
        return false;
    }

    auto&       asset              = json["assets"][0];
    std::string new_allay_jar_name = asset["name"];
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
    std::string download_url = asset["browser_download_url"];
    if (std::filesystem::exists(new_allay_jar_name)) {
        // That may because the last time user try to update
        // allay but interrupt the process. Should remove the old
        // file as we do not know if it is correct
        std::filesystem::remove(new_allay_jar_name);
    }
    std::ofstream output_file(new_allay_jar_name, std::ios::binary);
    if (!output_file) {
        logging::error("Can't create output file.");
        return false;
    }
    progresscpp::ProgressBar progress_bar(10000, 70);
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
    allay_launcher::util::file::clean_and_write_file(".current_allay_jar_name", new_allay_jar_name);
    logging::info("Successfully updated to {}.", new_allay_jar_name);

    return true;
}

void run_allay() {
    // TODO
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
        run_allay();
    }

    return 0;
}