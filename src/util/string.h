#pragma once

#include <charconv>

namespace allay_launcher::util::string {

AL_INLINE bool starts_with(std::string_view str, std::string_view prefix) {
    return prefix.size() <= str.size() && str.compare(0, prefix.size(), prefix) == 0;
}

AL_INLINE bool ends_with(std::string_view str, std::string_view suffix) {
    return suffix.size() <= str.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

AL_INLINE bool remove_prefix(std::string& str, std::string_view prefix) {
    if (str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0) {
        str.erase(0, prefix.size());
        return true;
    }
    return false;
}

AL_INLINE bool remove_prefix(std::string_view& str, std::string_view prefix) {
    if (str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0) {
        str.remove_prefix(prefix.size());
        return true;
    }
    return false;
}

AL_INLINE bool remove_suffix(std::string& str, std::string_view suffix) {
    if (str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0) {
        str.erase(str.size() - suffix.size());
        return true;
    }
    return false;
}

AL_INLINE bool remove_suffix(std::string_view& str, std::string_view suffix) {
    if (str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0) {
        str.remove_suffix(suffix.size());
        return true;
    }
    return false;
}

AL_INLINE std::optional<int32_t> to_int32(std::string_view str) {
    int32_t result = 0;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

    if (ec != std::errc()) {
        return {};
    }

    return result;
}

AL_INLINE std::vector<std::string_view> split_nocopy(std::string_view str, std::string_view delim) {
    std::vector<std::string_view> tokens;
    size_t                        start = 0, end = 0;

    while ((end = str.find(delim, start)) != std::string::npos) {
        tokens.emplace_back(str.substr(start, end - start));
        start = end + delim.length();
    }

    tokens.emplace_back(str.substr(start));

    return tokens;
}

AL_INLINE std::string_view extract_host(std::string_view url) {
    remove_prefix(url, "http://");
    remove_prefix(url, "https://");
    if (auto pos = url.find("/"); pos != std::string_view::npos) {
        return url.substr(0, pos);
    }
    return url;
}

AL_INLINE std::string uchar_array_to_hex(unsigned char data[], size_t len) {
    std::ostringstream oss;
    for (unsigned int i = 0; i < len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }
    return oss.str();
}

// from: https://stackoverflow.com/questions/2111667/compile-time-string-hashing
constexpr unsigned H(char const* input) {
    return *input ? static_cast<unsigned int>(*input) + 33 * H(input + 1) : 5381;
}

#ifdef _WIN32
AL_INLINE std::string from_wstring(const std::wstring& wstr) { return std::filesystem::path(wstr).string(); }
#endif

} // namespace allay_launcher::util::string
