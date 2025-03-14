#include <argparse/argparse.hpp>

#include "allay_server.h"

#include "config.h"

#include "util/java.h"

#ifdef _WIN32
#include "util/os.h" // set_console_cp_utf8
#endif

using namespace allay_launcher;

void setup_logger() {
#ifdef AL_DEBUG
    logging::set_level(logging::level::debug);
#endif
    logging::set_pattern("[%^%l%$] %v");
}

auto parse_arguments(int argc, char* argv[]) {
    using namespace argparse;

    ArgumentParser program("allay", ALLAY_LAUNCHER_VERSION);

    struct _args {
        static _args default_value() {
            _args args;
            args.m_run         = true;
            args.m_update      = true;
            args.m_use_nightly = false;
            args.m_deamon      = false;
            return args;
        }

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
    
    std::vector<std::string> raw_args;
    program.add_argument("-a", "--args")
        .help("Pass arguments to java")
        .nargs(nargs_pattern::at_least_one)
        .store_into(raw_args);

    program.parse_args(argc, argv);

    // clang-format on

    args.m_deamon = args.m_deamon && args.m_run;

    for (const auto& arg : raw_args) {
        args.m_extra_vm_args += "-" + arg + " ";
    }

    return args;
}

int main(int argc, char* argv[]) try {
#ifdef _WIN32
    util::os::set_console_cp_utf8();
#endif

    setup_logger();

    logging::info(
        "Allay launcher {} ({}).",
        format(fg(fmt::color::green), ALLAY_LAUNCHER_VERSION),
        format(fg(fmt::color::yellow), GIT_COMMIT)
    );

    if (!util::java::check_java()) {
        return -1;
    }

    auto args = parse_arguments(argc, argv);

    AllayServer server{"."};

    server.set_vm_extra_arguments(args.m_extra_vm_args);

    do {
        if (args.m_update) {
            try {
                server.update(args.m_use_nightly);
            } catch (const BaseException& e) {
                logging::error(e.what());
            }
        }

        if (args.m_run) {
            if (!server.run()) {
                // Failed to start the server. For example the allay jar file is not found
                break;
            }
        }

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