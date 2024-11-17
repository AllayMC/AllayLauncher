#pragma once

namespace allay_launcher {

enum class SimpleError { Ok, Failed };

enum class GetOSJavaVersionError { Ok, CommandNotFound, ExecuteCommandFailed, ParseVersionFailed };

enum class CommandExecutionError { Ok, CreatePipeError };

namespace error_util {

constexpr std::string to_string(SimpleError error) {
    switch (error) {
    case SimpleError::Ok:
        return "Everything is ok.";
    case SimpleError::Failed:
        return "Something went wrong.";
    }
    return "Unknown error.";
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
    return "Unknown error.";
}

constexpr std::string to_string(CommandExecutionError error) {
    switch (error) {
    case CommandExecutionError::Ok:
        return "Everything is ok.";
    case CommandExecutionError::CreatePipeError:
        return "Cannot create pipe.";
    }
    return "Unknown error.";
}

} // namespace error_util

} // namespace allay_launcher
