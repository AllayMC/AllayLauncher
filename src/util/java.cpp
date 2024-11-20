#include "java.h"

#include "util/os.h"
#include "util/string.h"

namespace allay_launcher::util::java {

Version installed_version() {
    using namespace string;

    std::string output;

    try {
        // Do not use "java --version", which is not supported on java-1.8
        // The output of this command will be written into error stream
        // so we need to add the suffix "2>&1" to redirect the output
        output = os::execute("java -version 2>&1");
    } catch (const CommandExecutionException& e) {
        throw GetOSJavaVersionException::ExecuteCommandFailed();
    }

    auto lines             = split(output, "\n");
    auto first_line_tokens = split(lines.at(0), " ");

    // openjdk version "21.0.4" 2024-07-16
    //                          ^ (optional)
    if (first_line_tokens.size() < 3) {
        throw GetOSJavaVersionException::ParseVersionFailed();
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
        throw GetOSJavaVersionException::ParseVersionFailed();
    }
}

bool check_java() {
    bool is_java_ok = false;

    Version version;

    try {
        version = util::java::installed_version();
    } catch (const GetOSJavaVersionException& e) {
        logging::error("Failed to check java version, please make sure if java is installed correctly.");
    }

    Version min_required_version{21, 0, 0};
    if (version < min_required_version) {
        logging::error("Unsupported java version: {}", version);
        logging::error("Please update your java to 21 or higher.");
    } else {
        is_java_ok = true;
    }

    if (!is_java_ok) {
        logging::error("Check https://docs.allaymc.org/getting-started/installation/#install-java");
        return false;
    }

    logging::info("Detected java version: {}", format(fg(fmt::color::green), "{}", version));
    return is_java_ok;
}

} // namespace allay_launcher::util::java
