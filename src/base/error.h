#pragma once

namespace allay_launcher {

enum class SimpleError { Ok, Failed };

enum class GetOSJavaVersionError { Ok, CommandNotFound, ExecuteCommandFailed, ParseVersionFailed };

enum class CommandExecutionError { Ok, CreatePipeError };

enum class DownloadFileError { Ok, FileExistsError, UnableToOpenFileError, NetworkError };

enum class UpdateAllayError { Ok, GetReleaseError, WrongAssertCount, WrongFileName, DownloadFileError }; // TODO

namespace error_util {

constexpr std::string to_string(SimpleError error) {
    switch (error) {
    case SimpleError::Ok:
        return "Everything is ok.";
    case SimpleError::Failed:
        return "Something went wrong.";
    }
    return "SimpleError: Unknown error.";
}

constexpr std::string to_string(GetOSJavaVersionError error) {
    switch (error) {
    case GetOSJavaVersionError::Ok:
        return "Everything is ok.";
    case GetOSJavaVersionError::CommandNotFound:
        return "Command not found.";
    case GetOSJavaVersionError::ExecuteCommandFailed:
        return "Execute command failed.";
    case GetOSJavaVersionError::ParseVersionFailed:
        return "Parse java version failed.";
    }
    return "GetOSJavaVersionError: Unknown error.";
}

constexpr std::string to_string(CommandExecutionError error) {
    switch (error) {
    case CommandExecutionError::Ok:
        return "Everything is ok.";
    case CommandExecutionError::CreatePipeError:
        return "Cannot create pipe.";
    }
    return "CommandExecutionError: Unknown error.";
}

constexpr std::string to_string(DownloadFileError error) {
    switch (error) {
    case DownloadFileError::Ok:
        return "Everything is ok.";
    case DownloadFileError::FileExistsError:
        return "Can't create output file.";
    case DownloadFileError::UnableToOpenFileError:
        return "Can't open file.";
    case DownloadFileError::NetworkError:
        return "Network error.";
    }
    return "DownloadFileError: Unknown error.";
}

constexpr std::string to_string(UpdateAllayError error) {
    switch (error) {
    case UpdateAllayError::Ok:
        return "Everything is ok.";
    case UpdateAllayError::GetReleaseError:
        return "Error while getting release information.";
    case UpdateAllayError::WrongAssertCount:
        return "Wrong assert count error.";
    case UpdateAllayError::WrongFileName:
        return "Wrong assert file name.";
    case UpdateAllayError::DownloadFileError:
        return "Error while downloading file.";
    };
    return "UpdateAllayError: Unknown error.";
}

} // namespace error_util

} // namespace allay_launcher
