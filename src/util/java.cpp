#include "java.h"

#include "util/os.h"
#include "util/string.h"

namespace allay_launcher::util::java {

Version installed_version() {
    using namespace string;

    // Do not use "java --version", which is not supported on java-1.8
    // The output of this command will be written into error stream
    // so we need to add the suffix "2>&1" to redirect the output
    auto output = os::execute("java -version 2>&1");

    auto lines             = split_nocopy(output, "\n");
    auto first_line_tokens = split_nocopy(lines.at(0), " ");

    // openjdk version "21.0.4" 2024-07-16
    //                          ^ (optional)
    if (first_line_tokens.size() < 3) {
        throw ParserException(output, "unable to parse java version.");
    }

    auto version_string = first_line_tokens.at(2);
    remove_prefix(version_string, "\"");
    remove_suffix(version_string, "\"");

    // openjdk version "1.8.0_432"
    if (auto underline_pos = version_string.find("_"); underline_pos != std::string::npos) {
        version_string = version_string.substr(0, underline_pos);
    }

    if (auto ret = Version::parse(version_string)) {
        return *ret;
    } else {
        throw ParserException(version_string, "unable to parse version string.");
    }
}

bool check_java() try {
    bool is_java_ok = false;

    auto version = util::java::installed_version();

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

} catch (const ParserException& e) {
    logging::error(e.what());
    logging::error("Failed to check java version, please make sure if java is installed correctly.");
    return false;
} catch (const CommandExecutionException& e) {
    logging::error(e.what());
    return false;
}

} // namespace allay_launcher::util::java
