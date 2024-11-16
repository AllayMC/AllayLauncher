#include "java.h"

#include "util/os.h"
#include "util/string.h"

namespace allay_launcher::util::java {

std::expected<Version, GetOSJavaVersionError> installed_version() {
    using namespace string;

    // Do not use "java --version", which is not supported on java-1.8
    // The output of this command will be written into error stream
    // so we need to add the suffix "2>&1" to redirect the output
    auto output = os::execute("java -version 2>&1");

    if (!output) return std::unexpected(GetOSJavaVersionError::ExecuteCommandFailed);

    auto lines             = split(*output, "\n");
    auto first_line_tokens = split(lines.at(0), " ");

    // openjdk version "21.0.4" 2024-07-16
    //                          ^ (optional)
    if (first_line_tokens.size() < 3) {
        return std::unexpected(GetOSJavaVersionError::ParseVersionFailed);
    }

    auto version_string = first_line_tokens.at(2);
    string::remove_prefix(version_string, "\"");
    string::remove_suffix(version_string, "\"");

    // openjdk version "1.8.0_432"
    if (auto underline_pos = version_string.find("_"); underline_pos != std::string::npos) {
        version_string = version_string.substr(0, underline_pos);
    }

    if (auto ret = Version::parse(version_string)) {
        return *ret;
    } else {
        return std::unexpected(GetOSJavaVersionError::ParseVersionFailed);
    }
}

} // namespace allay_launcher::util::java
