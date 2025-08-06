#pragma once

namespace allay_launcher::util::terminal {

class Spinner {
public:
    Spinner();
    ~Spinner();

    Spinner(Spinner const&)            = delete;
    Spinner(Spinner&&)                 = delete;
    Spinner& operator=(Spinner const&) = delete;
    Spinner& operator=(Spinner&&)      = delete;

    using scope_func   = std::function<void()>;
    using printer_func = std::function<void(std::string_view spinner, std::string_view hint)>;

    Spinner& hint(std::string_view hint);
    Spinner& hint_end(std::string_view hint);

    Spinner& interval(int interval);
    Spinner& printer(printer_func&& cb);
    Spinner& printer_end(printer_func&& cb);
    Spinner& scope(scope_func&& cb);

    void run();

private:
    std::string  m_hint;
    std::string  m_hint_end;
    int          m_interval;
    scope_func   m_scope;
    printer_func m_printer;
    printer_func m_printer_end;

    std::atomic<bool> m_running{false};
    std::thread       m_thread;

    const std::vector<std::string>* CHARSET{};

    static const std::vector<std::string> CHARSET_UNICODE;
    static const std::vector<std::string> CHARSET_LEGACY_WINDOWS;

    static void _default_printer(std::string_view spinner, std::string_view hint);
    static void _default_printer_end(std::string_view spinner, std::string_view hint);
};

} // namespace allay_launcher::util::terminal
