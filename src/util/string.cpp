#include "string.h"

namespace allay_launcher::util::string {

std::expected<int32_t, SimpleError> to_int32(std::string_view str) {
    char* str_end = nullptr;
    errno         = 0;

    long value = std::strtol(str.data(), &str_end, 0);

    if (str_end == str.data() || errno == ERANGE || *str_end != '\0') {
        return std::unexpected(SimpleError::Failed);
    }

    return value;
}

void remove_prefix(std::string& str, const std::string& prefix) {
    if (str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0) {
        str.erase(0, prefix.size());
    }
}

void remove_suffix(std::string& str, const std::string& suffix) {
    if (str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0) {
        str.erase(str.size() - suffix.size());
    }
}

std::vector<std::string> split(std::string_view str, std::string_view delimiter) {
    std::vector<std::string> tokens;
    size_t                   start = 0, end = 0;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        tokens.emplace_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }

    tokens.emplace_back(str.substr(start));

    return tokens;
}

} // namespace allay_launcher::util::string
