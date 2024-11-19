#include "github/internal/api_base.h"

#include "util/os.h"
#include "util/string.h"

namespace allay_launcher::github {

std::shared_ptr<cpr::Session> ApiBase::create_session() const {
    auto session = std::make_shared<cpr::Session>();
#ifdef _WIN32
    auto system_proxy = util::os::configured_proxy_info();
    logging::debug("Proxy used: {}", system_proxy->m_proxy_server);
    session->SetProxies({
        {"http",  system_proxy->m_proxy_server},
        {"https", system_proxy->m_proxy_server}
    });
#endif
    // TODO: Should we use GetSharedPtrFromThis?
    //       see: https://github.com/libcpr/cpr/issues/1145
    return session->shared_from_this();
}

} // namespace allay_launcher::github