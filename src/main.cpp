#include <argparse/argparse.hpp>

#include "spdlog/common.h"
#include "spdlog/fmt/bundled/color.h"
#include "spdlog/spdlog.h"

#include "config.h"
#include "util/java.h"


using namespace allay_launcher;
using logging::fmt_lib::color;

void setup_logger() {
#ifdef DEBUG
    logging::set_level(spdlog::level::debug);
#endif
    logging::set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
}

bool check_java() {
    bool is_java_ok = false;

    auto version = util::java::installed_version();
    if (version) {
        Version min_required_version{21, 0, 0};
        if (*version < min_required_version) {
            logging::error("Unsupported java version: {}", static_cast<std::string>(*version));
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

    logging::info("Detected java version: {}", format(fg(color::green), static_cast<std::string>(*version)));
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
            .flag();

    program.parse_args(argc, argv);

    struct {
        bool m_update;
        bool m_use_nightly;
    } ret {
        program.get<bool>("--update"), 
        program.get<bool>("--nightly")
    };

    // clang-format on

    return ret;
}

int main(int argc, char* argv[]) {
    logging::info("Allay launcher version: {} ({})", format(fg(color::green), ALLAY_LAUNCHER_VERSION), format(fg(color::yellow), GIT_COMMIT));
    setup_logger();

    if (!check_java()) return -1;

    auto args = parse_arguments(argc, argv);

    // TODO

    if (args.m_update) {}


    return 0;
}
