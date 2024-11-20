#pragma once
#include <cpr/cpr.h>
namespace allay_launcher::util::network {

void download(cpr::Session& session, std::string_view download_url, std::string_view save_path);

} // namespace allay_launcher::util::network