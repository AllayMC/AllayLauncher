#include "util/os.h"

namespace allay_launcher::util::os {

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

bool is_legacy_windows() { return false; }

} // namespace allay_launcher::util::os
