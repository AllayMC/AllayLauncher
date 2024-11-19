#pragma once

#include <cpr/cpr.h>

namespace allay_launcher::github {

class ApiBase {
public:
    explicit ApiBase(std::string_view api_base) : m_url(api_base) {};

    virtual std::shared_ptr<cpr::Session> create_session() const;

protected:
    virtual cpr::Url build_url() const { return m_url; }

private:
    cpr::Url m_url;
};

} // namespace allay_launcher::github