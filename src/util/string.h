#pragma once

namespace allay_launcher::util::string {

std::expected<int32_t, SimpleError> to_int32(std::string_view str);

void remove_prefix(std::string& str, const std::string& prefix);

void remove_suffix(std::string& str, const std::string& suffix);

std::vector<std::string> split(std::string_view str, std::string_view delimiter);

#ifdef _WIN32
std::string from_wstring(const std::wstring& wstr);
#endif

// from: https://stackoverflow.com/questions/2111667/compile-time-string-hashing
constexpr unsigned H(char const* input) {
    return *input ? static_cast<unsigned int>(*input) + 33 * H(input + 1) : 5381;
}

} // namespace allay_launcher::util::string
