#include "base/global.h"

#include "util/os.h"

#include <cpr/cpr.h>

namespace allay_launcher::global {

std::shared_ptr<cpr::Session> CreateCprSession() {
    auto session = std::make_shared<cpr::Session>();


    if (auto system_proxy = util::os::system_proxy_configuration()) {
        logging::debug("Proxy used: {}", system_proxy->m_server);
        session->SetProxies({
            {"http",  system_proxy->m_server},
            {"https", system_proxy->m_server}
        });
    }

    // TODO: Should we use GetSharedPtrFromThis?
    //       see: https://github.com/libcpr/cpr/issues/1145
    return session->shared_from_this();
}

} // namespace allay_launcher::global
