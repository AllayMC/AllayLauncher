#pragma once

#include "base/version.h"

namespace allay_launcher::util::java {

/* @Throwable ParserException
 * @Throwable CommandExecutionException
 */
Version installed_version();

bool check_java();

} // namespace allay_launcher::util::java
