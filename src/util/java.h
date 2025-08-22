#pragma once

#include "base/version.h"

namespace allay_launcher::util::java {

Version installed_version(std::string_view cmdoutput);

/* @Throwable ParserException
 * @Throwable CommandExecutionException
 */
Version installed_version();

bool check_java();

} // namespace allay_launcher::util::java
