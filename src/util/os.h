#pragma once

namespace allay_launcher::util::os {

struct SystemProxyInfo {
    bool        m_auto_detect;
    std::string m_auto_config_url;
    std::string m_proxy_server;
    std::string m_proxy_bypass;
};

std::expected<std::string, CommandExecutionError> execute(std::string_view command);

void system(std::string_view command);

#ifdef _WIN32
std::expected<SystemProxyInfo, SimpleError> configured_proxy_info();
#endif

} // namespace allay_launcher::util::os
