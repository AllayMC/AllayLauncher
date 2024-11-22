#pragma once

#include <cpr/cpr.h>

namespace allay_launcher::github {

class AddHeaderInterceptor : public cpr::Interceptor {
public:
    cpr::Response intercept(cpr::Session& session) override {
        session.SetHeader({
            {"Accept",               "application/vnd.github+json"},
            // {"Authorization", "Bearer <YOUR-TOKEN>"}
            {"X-GitHub-Api-Version", "2022-11-28"                 }
        });

        return proceed(session);
    }
};

} // namespace allay_launcher::github
