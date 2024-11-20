#include "os.h"

#include "internal/popen.h"

namespace allay_launcher::util::os {

using namespace internal;

std::string execute(std::string_view command) {
    logging::debug("execute(): {}", command);

    Popen pipe(command, "r");
    if (!pipe.get()) throw CommandExecutionException::CreatePipeError();

    std::array<char, 128> buffer;
    std::string           ret;
    while (fgets(buffer.data(), buffer.size(), pipe.get())) ret += buffer.data();

    return ret;
}

void system(std::string_view command) {
    logging::debug("system(): {}", command);

    ::system(command.data());
}

} // namespace allay_launcher::util::os
