#pragma once

namespace allay_launcher::github {

enum class GetReleaseError { Ok, NetworkError, JsonError, NotFound };

inline namespace error_util {

constexpr std::string to_string(GetReleaseError error) {
    switch (error) {
    case GetReleaseError::Ok:
        return "Everything is ok.";
    case GetReleaseError::NetworkError:
        return "Network error.";
    case GetReleaseError::JsonError:
        return "Json error.";
    case GetReleaseError::NotFound:
        return "Not found.";
    }
    return "Unknown error.";
}

} // namespace error_util

} // namespace allay_launcher::github