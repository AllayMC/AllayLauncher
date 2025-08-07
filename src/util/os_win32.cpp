#include "util/os.h"
#include "util/string.h"

#include <windows.h>
#include <winhttp.h>

#include <versionhelpers.h>

namespace allay_launcher::util::os {

std::optional<SystemProxy> system_proxy_configuration() {
    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG _raw_system_config;
    if (!WinHttpGetIEProxyConfigForCurrentUser(&_raw_system_config)) {
        return {};
    }

    if (_raw_system_config.lpszProxy) {
        SystemProxy config;
        config.m_server = string::from_wstring(_raw_system_config.lpszProxy);
        GlobalFree(_raw_system_config.lpszProxy);
        return config;
    }

    return {};
}

void set_console_cp_utf8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

bool is_legacy_windows() { return !IsWindows10OrGreater(); }

} // namespace allay_launcher::util::os
