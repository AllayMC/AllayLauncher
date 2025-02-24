#pragma once

namespace allay_launcher::util::string {

AL_INLINE bool starts_with(const std::string& str, const std::string& prefix) {
    return prefix.size() <= str.size() && str.compare(0, prefix.size(), prefix) == 0;
}

AL_INLINE bool ends_with(const std::string& str, const std::string& suffix) {
    return suffix.size() <= str.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

AL_INLINE void remove_prefix(std::string& str, const std::string& prefix) {
    if (str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0) {
        str.erase(0, prefix.size());
    }
}

AL_INLINE void remove_suffix(std::string& str, const std::string& suffix) {
    if (str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0) {
        str.erase(str.size() - suffix.size());
    }
}

AL_INLINE std::optional<int32_t> to_int32(std::string_view str) {
    char* str_end = nullptr;
    errno         = 0;

    long value = std::strtol(str.data(), &str_end, 0);

    if (str_end == str.data() || errno == ERANGE || *str_end != '\0') {
        return {};
    }

    return value;
}

AL_INLINE std::vector<std::string> split(std::string_view str, std::string_view delimiter) {
    std::vector<std::string> tokens;
    size_t                   start = 0, end = 0;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        tokens.emplace_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }

    tokens.emplace_back(str.substr(start));

    return tokens;
}

// from: https://stackoverflow.com/questions/2111667/compile-time-string-hashing
constexpr unsigned H(char const* input) {
    return *input ? static_cast<unsigned int>(*input) + 33 * H(input + 1) : 5381;
}

#ifdef _WIN32
AL_INLINE std::string from_wstring(const std::wstring& wstr) { return std::filesystem::path(wstr).string(); }
#endif

} // namespace allay_launcher::util::string
