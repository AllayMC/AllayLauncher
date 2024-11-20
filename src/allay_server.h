#pragma once

namespace allay_launcher {

class AllayServer {
public:
    explicit AllayServer(std::string_view path) : m_path(path) {};

    AllayServer(const AllayServer& other)    = delete;
    auto operator=(const AllayServer& other) = delete;

    void set_vm_extra_arguments(std::string_view args) { m_vm_extra_arguments = args; };

    bool run();

    void update(bool use_nightly = false);

private:
    std::string m_path; // TODO

    std::string m_vm_extra_arguments;
};

} // namespace allay_launcher