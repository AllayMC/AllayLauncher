#pragma once

namespace allay_launcher {

class BaseException : public std::exception {
public:
    explicit BaseException(std::string_view str) : m_what(str) {}

    const char* what() const noexcept override { return m_what.c_str(); }

private:
    std::string m_what;
};

class ConnectionException : public BaseException {
public:
    explicit ConnectionException(int error_code, int status_code, std::string_view message, std::string_view url)
    : BaseException(
          fmt::format("[exception.connection.{}] (status_code: {}, url: {}) {}", error_code, status_code, url, message)
      ) {}
};

class IOException : public BaseException {
public:
    explicit IOException(std::string_view file, std::string_view message)
    : BaseException(fmt::format("[exception.io] (file: {}) {}", file, message)) {}
};

class CommandExecutionException : public BaseException {
public:
    explicit CommandExecutionException(std::string_view cmd, std::string_view message)
    : BaseException(fmt::format("[exception.command] (cmd: {}) {}", cmd, message)) {}
};

class ParserException : public BaseException {
public:
    explicit ParserException(std::string_view raw_str, std::string_view message)
    : BaseException(fmt::format("[exception.parser] (str: {}) {}", raw_str, message)) {}
};

class NeedUpdateException : public BaseException {
public:
    explicit NeedUpdateException(std::string_view extra_info)
    : BaseException(fmt::format(
          "[exception.upstream] We ran into a situation we can't handle, please check for updates, "
          "or open an issue. ({})",
          extra_info
      )) {}
};

class JsonException : public BaseException {
public:
    explicit JsonException(std::string_view error_message, std::string_view json_str)
    : BaseException(fmt::format("{}\n{}", error_message, json_str)) {}
};

class NothingException : public BaseException {
public:
    explicit NothingException() : BaseException("[exception.nothing] We can't find anything.") {}
};

class CryptoException : public BaseException {
public:
    explicit CryptoException(std::string_view method)
    : BaseException(fmt::format("[exception.openssl] An exception occurred while executing {}", method)) {}
};

class ChecksumException : public BaseException {
public:
    explicit ChecksumException(std::string_view reason, const std::filesystem::path& path)
    : BaseException(fmt::format("[exception.integrity] Unable to verify integrity, {}.", reason)) {
        std::filesystem::remove(path);
    }
};

} // namespace allay_launcher
