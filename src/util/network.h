#pragma once

#include <cpr/cpr.h>

namespace allay_launcher::util::network {

/* @Throwable IOException
 * @Throwable ConnectionException
 */
void download(std::string_view download_url, std::string_view save_path);

} // namespace allay_launcher::util::network