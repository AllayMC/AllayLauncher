#include "util/terminal.h"

#include "util/os.h"

namespace allay_launcher::util::terminal {

const std::vector<std::string> Spinner::CHARSET_UNICODE = {"✔", "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
const std::vector<std::string> Spinner::CHARSET_LEGACY_WINDOWS = {"OK", "\\", "-", "/", "|"};

Spinner::Spinner()
: m_printer(_default_printer),
  m_printer_end(_default_printer_end),
  m_scope([]() {}),
  m_interval(100) {
    if (!os::is_legacy_windows()) {
        CHARSET = &CHARSET_UNICODE;
    } else {
        CHARSET = &CHARSET_LEGACY_WINDOWS;
    }
}

Spinner::~Spinner() {
    if (!m_running) return;
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
    m_printer_end((*CHARSET)[0], m_hint_end);
    std::cout.flush();
}

Spinner& Spinner::hint(std::string_view hint) {
    m_hint = hint;
    return *this;
}

Spinner& Spinner::hint_end(std::string_view hint) {
    m_hint_end = hint;
    return *this;
}

Spinner& Spinner::interval(int interval) {
    m_interval = interval;
    return *this;
}

Spinner& Spinner::printer(printer_func&& cb) {
    m_printer = std::move(cb);
    return *this;
}

Spinner& Spinner::printer_end(printer_func&& cb) {
    m_printer_end = std::move(cb);
    return *this;
}

Spinner& Spinner::scope(scope_func&& cb) {
    m_scope = std::move(cb);
    return *this;
}

void Spinner::run() {
    if (m_running) return;
    m_running = true;
    m_thread  = std::thread([this]() {
        size_t index = 0;
        while (m_running) {
            auto c_index = index++ % CHARSET->size();

            // CHARSET[0] is the completion mark.
            if (c_index == 0) c_index = 1;

            auto& spinner = (*CHARSET)[c_index];
            m_printer(spinner, m_hint);
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
        }
    });
    m_scope();
}

void Spinner::_default_printer(std::string_view spinner, std::string_view hint) {
    fmt::print("\r  {} {}", spinner, hint);
}

void Spinner::_default_printer_end(std::string_view spinner, std::string_view hint) {
    if (!hint.empty()) {
        fmt::print("\r  {} {}{}\n", spinner, hint, std::string(10, ' '));
    } else {
        fmt::print("\r");
    }
}

} // namespace allay_launcher::util::terminal
