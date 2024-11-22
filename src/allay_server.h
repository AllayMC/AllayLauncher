#pragma once

namespace allay_launcher {

class AllayServer {
public:
    explicit AllayServer(std::string_view path) : m_path(path) {};

    AllayServer(const AllayServer& other)    = delete;
    auto operator=(const AllayServer& other) = delete;

    void set_vm_extra_arguments(std::string_view args) { m_vm_extra_arguments = args; };

    bool run();

    /* @Throwable IOException
     * @Throwable ConnectionException
     * @Throwable NeedUpdateException
     * @Throwable github
     */
    void update(bool use_nightly = false);

private:
    std::string m_path;

    std::string m_vm_extra_arguments;

    std::string _current_jar_name() const;
    void        _current_jar_name(std::string_view new_current_jar_name);
};

} // namespace allay_launcher