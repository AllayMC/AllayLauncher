#pragma once

namespace allay_launcher::util::string {

bool starts_with(const std::string& str, const std::string& prefix);

bool ends_with(const std::string& str, const std::string& suffix);

void remove_prefix(std::string& str, const std::string& prefix);

void remove_suffix(std::string& str, const std::string& suffix);

std::optional<int32_t> to_int32(std::string_view str);

std::vector<std::string> split(std::string_view str, std::string_view delimiter);

// from: https://stackoverflow.com/questions/2111667/compile-time-string-hashing
constexpr unsigned H(char const* input) {
    return *input ? static_cast<unsigned int>(*input) + 33 * H(input + 1) : 5381;
}

#ifdef _WIN32
std::string from_wstring(const std::wstring& wstr);
#endif

} // namespace allay_launcher::util::string
