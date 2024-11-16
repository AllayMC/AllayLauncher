#pragma once

namespace allay_launcher::util::os {

std::expected<std::string, CommandExecutionError> execute(std::string_view command);

} // namespace allay_launcher::util::os
