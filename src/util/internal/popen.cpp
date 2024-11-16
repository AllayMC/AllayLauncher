#include "popen.h"

#ifdef _WIN32
#define popen  _popen
#define pclose _pclose
#endif

namespace allay_launcher::util::internal {

Popen::Popen(std::string_view command, std::string_view mode) { m_pipe = popen(command.data(), mode.data()); }

void Popen::close() {
    if (m_pipe) {
        pclose(m_pipe);
        m_pipe = nullptr;
    }
}

} // namespace allay_launcher::util::internal
