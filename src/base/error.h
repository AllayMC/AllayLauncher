#pragma once

namespace allay_launcher {

class GetOSJavaVersionException : public std::exception {
public:
    static auto CommandNotFound() { return GetOSJavaVersionException{_CommandNotFound}; }
    static auto ExecuteCommandFailed() { return GetOSJavaVersionException{_ExecuteCommandFailed}; }
    static auto ParseVersionFailed() { return GetOSJavaVersionException{_ParseVersionFailed}; }

    const char* what() const noexcept override {
        switch (m_error_code) {
        case _CommandNotFound:
            return "Command not found.";
        case _ExecuteCommandFailed:
            return "Execute command failed.";
        case _ParseVersionFailed:
            return "Parse java version failed.";
        }
        return "Unknown error.";
    }

private:
    enum error_code { _CommandNotFound, _ExecuteCommandFailed, _ParseVersionFailed } m_error_code;

    explicit GetOSJavaVersionException(error_code code) : m_error_code(code) {}
};

class CommandExecutionException : public std::exception {
public:
    static auto CreatePipeError() { return CommandExecutionException{_CreatePipeError}; }

    const char* what() const noexcept override {
        switch (m_error_code) {
        case _CreatePipeError:
            return "Cannot create pipe.";
        }
        return "Unknown error.";
    }

private:
    enum error_code { _CreatePipeError } m_error_code;

    explicit CommandExecutionException(error_code code) : m_error_code(code) {}
};

class DownloadFileException : public std::exception {
public:
    static auto FileExistsError() { return DownloadFileException{_FileExistsError}; }
    static auto UnableToOpenFileError() { return DownloadFileException{_UnableToOpenFileError}; }
    static auto NetworkError() { return DownloadFileException{_NetworkError}; }

    const char* what() const noexcept override {
        switch (m_error_code) {
        case _FileExistsError:
            return "Can't create output file.";
        case _UnableToOpenFileError:
            return "Can't open file.";
        case _NetworkError:
            return "Network error.";
        }
        return "Unknown error.";
    }

private:
    enum error_code { _FileExistsError, _UnableToOpenFileError, _NetworkError } m_error_code;

    explicit DownloadFileException(error_code code) : m_error_code(code) {}
};

class UpdateAllayException : public std::exception {
public:
    static auto GetReleaseError() { return UpdateAllayException{_GetReleaseError}; }
    static auto WrongAssertCount() { return UpdateAllayException{_WrongAssertCount}; }
    static auto WrongFileName() { return UpdateAllayException{_WrongFileName}; }
    static auto DownloadFileError() { return UpdateAllayException{_DownloadFileError}; }

    const char* what() const noexcept override {
        switch (m_error_code) {
        case _GetReleaseError:
            return "Error while getting release information.";
        case _WrongAssertCount:
            return "Wrong assert count error.";
        case _WrongFileName:
            return "Wrong assert file name.";
        case _DownloadFileError:
            return "Error while downloading file.";
        }
        return "Unknown error.";
    }

private:
    enum error_code { _GetReleaseError, _WrongAssertCount, _WrongFileName, _DownloadFileError } m_error_code;

    explicit UpdateAllayException(error_code code) : m_error_code(code) {}
};

} // namespace allay_launcher
