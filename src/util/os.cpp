#include "os.h"

#include "internal/popen.h"

namespace allay_launcher::util::os {

using namespace internal;

std::expected<std::string, CommandExecutionError> execute(std::string_view command) {
    Popen pipe(command, "r");
    if (!pipe.get()) return std::unexpected(CommandExecutionError::CreatePipeError);

    std::array<char, 128> buffer;
    std::string           ret;
    while (fgets(buffer.data(), buffer.size(), pipe.get())) ret += buffer.data();

    return ret;
}

} // namespace allay_launcher::util::os
