#pragma once

namespace allay_launcher::util::os {

struct SystemProxy {
    std::string m_server;
};

std::string execute(std::string_view command);

int run(std::string_view command);

std::string environment(std::string_view var);

std::optional<SystemProxy> system_proxy_configuration();

bool is_legacy_windows();

#ifdef _WIN32
void set_console_cp_utf8();
#endif

} // namespace allay_launcher::util::os
