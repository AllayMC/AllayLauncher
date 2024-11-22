#include "github/internal/api_base.h"
#include "github/internal/interceptor.h"

namespace allay_launcher::github {

std::shared_ptr<cpr::Session> ApiBase::create_session() const {
    auto session = global::CreateCprSession();
    session->AddInterceptor(std::make_shared<AddHeaderInterceptor>());
    return session;
}

} // namespace allay_launcher::github