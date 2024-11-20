#include <argparse/argparse.hpp>

#include "allay_server.h"
#include "config.h"

#include "spdlog/spdlog.h"
#include "util/string.h"

#include "util/os.h"

using namespace allay_launcher;

void setup_logger() {
#ifdef DEBUG
    logging::set_level(spdlog::level::debug);
#endif
    logging::set_pattern("[%^%l%$] %v");
}

auto parse_arguments(int argc, char* argv[]) {
    using namespace argparse;

    ArgumentParser program("allay", "1.0.0-BETA");

    struct _args {
        static _args default_value() { return {.m_run = true, .m_update = true, .m_use_nightly = true}; }

        bool m_run;
        bool m_update;
        bool m_use_nightly;
        bool m_deamon;

        std::string m_extra_vm_args;
    } args;

    if (argc == 1) return _args::default_value();

    // clang-format off

    program.add_argument("-u", "--update")
        .help("Update allay")
        .flag()
        .store_into(args.m_update);
    
    program.add_argument("-n", "--nightly")
        .help("Use nightly build")
        .flag()
        .store_into(args.m_use_nightly);
    
    program.add_argument("-r", "--run")
        .help("Run allay server")
        .flag()
        .store_into(args.m_run);

    program.add_argument("-d", "--deamon")
        .help("Use deamon mode, which the launcher will restart the server after server stopped.")
        .flag()
        .store_into(args.m_deamon);
    
    program.add_argument("-a", "--args")
        .help("Pass arguments to java")
        .store_into(args.m_extra_vm_args);

    program.parse_args(argc, argv);

    // clang-format on

    return args;
}

int main(int argc, char* argv[]) try {
#ifdef _WIN32
    util::os::system("chcp 65001");
#endif
    setup_logger();

    logging::info(
        "Allay launcher {} ({}).",
        format(fg(fmt::color::green), ALLAY_LAUNCHER_VERSION),
        format(fg(fmt::color::yellow), GIT_COMMIT)
    );

    auto args = parse_arguments(argc, argv);

    AllayServer server{"."};

    util::string::remove_prefix(args.m_extra_vm_args, "'");
    util::string::remove_suffix(args.m_extra_vm_args, "'");
    util::string::remove_prefix(args.m_extra_vm_args, "\"");
    util::string::remove_suffix(args.m_extra_vm_args, "\"");

    server.set_vm_extra_arguments(args.m_extra_vm_args);

    do {
        if (args.m_update) {
            auto result = server.update(args.m_use_nightly);
            if (!result) {
                logging::error(error_util::to_string(result.error()));
                return -1;
            }
        }

        if (args.m_run) {
            server.run();
        }

        args.m_deamon = args.m_deamon && args.m_run;
        if (args.m_deamon) {
            logging::info("Deamon mode is enabled, server will be restarted in 5 seconds. Use Ctrl+C to interrupt.");
            std::this_thread::sleep_for(std::chrono::seconds(5));
            logging::info("Restart the server...");
        }
    } while (args.m_deamon);

    return 0;
} catch (const std::exception& e) {
    logging::error(e.what());
    return -1;
}