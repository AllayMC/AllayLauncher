#include "github/internal/api_base.h"

namespace allay_launcher::github {

cpr::Session ApiBase::create_session() const {
    cpr::Session session;
    return session;
}

} // namespace allay_launcher::github