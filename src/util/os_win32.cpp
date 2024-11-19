#include "util/os.h"
#include "util/string.h"

#ifdef _WIN32
#include <windows.h>
#include <winhttp.h>

namespace allay_launcher::util::os {

std::expected<SystemProxyInfo, SimpleError> configured_proxy_info() {
    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxy_config;
    if (!WinHttpGetIEProxyConfigForCurrentUser(&proxy_config)) {
        return std::unexpected(SimpleError::Failed);
    }

    SystemProxyInfo info;

    info.m_auto_detect = proxy_config.fAutoDetect;

    if (proxy_config.lpszAutoConfigUrl) {
        info.m_auto_config_url = string::from_wstring(proxy_config.lpszAutoConfigUrl);
        GlobalFree(proxy_config.lpszAutoConfigUrl);
    }

    if (proxy_config.lpszProxy) {
        info.m_proxy_server = string::from_wstring(proxy_config.lpszProxy);
        GlobalFree(proxy_config.lpszProxy);
    }

    if (proxy_config.lpszProxyBypass) {
        info.m_proxy_bypass = string::from_wstring(proxy_config.lpszProxyBypass);
        GlobalFree(proxy_config.lpszProxyBypass);
    }

    return info;
}

} // namespace allay_launcher::util::os
#endif
