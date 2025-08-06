#pragma once

#include <spdlog/spdlog.h>

#include <spdlog/fmt/bundled/color.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace allay_launcher {

namespace logging = spdlog; // NOLINT

class UppercaseLevelFormatter : public logging::custom_flag_formatter {
public:
    void format(const logging::details::log_msg& msg, const std::tm&, logging::memory_buf_t& dest) override {
        std::string level_name;

        switch (msg.level) {
        case logging::level::trace:
            level_name = "TRACE";
            break;
        case logging::level::debug:
            level_name = "DEBG";
            break;
        case logging::level::info:
            level_name = "INFO";
            break;
        case logging::level::warn:
            level_name = "WARN";
            break;
        case logging::level::err:
            level_name = "ERROR";
            break;
        case logging::level::critical:
            level_name = "CRITICAL";
            break;
        case logging::level::off:
            level_name = "OFF";
            break;
        default:
            level_name = "UNKNOWN";
            break;
        }

        dest.append(level_name);
    }

    std::unique_ptr<custom_flag_formatter> clone() const override {
        return logging::details::make_unique<UppercaseLevelFormatter>();
    }
};

} // namespace allay_launcher
