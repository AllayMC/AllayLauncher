#include "github/internal/api_base.h"

namespace allay_launcher::github {

std::shared_ptr<cpr::Session> ApiBase::create_session() const {
    auto session = std::make_shared<cpr::Session>();

    // TODO: Should we use GetSharedPtrFromThis?
    //       see: https://github.com/libcpr/cpr/issues/1145
    return session->shared_from_this();
}

} // namespace allay_launcher::github