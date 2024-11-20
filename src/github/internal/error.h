#pragma once

namespace allay_launcher::github {

class GetReleaseException : public std::exception {
public:
    static auto NetworkError() { return GetReleaseException{_NetworkError}; }
    static auto JsonError() { return GetReleaseException{_JsonError}; }
    static auto NotFound() { return GetReleaseException{_NotFound}; }

    const char* what() const noexcept override {
        switch (m_error_code) {
        case _NetworkError:
            return "Network error.";
        case _JsonError:
            return "Json error.";
        case _NotFound:
            return "Not found.";
        }
        return "Unknown error.";
    }

private:
    enum error_code { _NetworkError, _JsonError, _NotFound } m_error_code;

    explicit GetReleaseException(error_code code) : m_error_code(code) {}
};

} // namespace allay_launcher::github