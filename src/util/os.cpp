#include "util/os.h"

#include "internal/popen.h"

namespace allay_launcher::util::os {

using namespace internal;

std::string execute(std::string_view command) {
    logging::debug("execute(): {}", command);

    Popen pipe(command, "r");
    if (!pipe.get()) throw CommandExecutionException(command, "cannot create pipe.");

    std::array<char, 128> buffer;
    std::string           ret;
    while (fgets(buffer.data(), buffer.size(), pipe.get())) ret += buffer.data();

    return ret;
}

int run(std::string_view command) {
    logging::debug("run(): {}", command);

    return std::system(command.data());
}

std::string environment(std::string_view var) {
    auto val = std::getenv(var.data());
    return val ? std::string{val} : std::string{};
}

std::optional<SystemProxy> system_proxy_configuration() {
    static const std::vector<std::string> vars{"https_proxy", "HTTPS_PROXY", "http_proxy", "HTTP_PROXY"};

    for (auto& var : vars) {
        if (auto env = environment(var); !env.empty()) {
            SystemProxy config;
            config.m_server = env;
            return config;
        }
    }

    return {};
}

} // namespace allay_launcher::util::os
