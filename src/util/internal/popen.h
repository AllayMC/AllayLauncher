#pragma once

namespace allay_launcher::util::internal {

class Popen {
public:
    Popen(std::string_view command, std::string_view mode);

    Popen(Popen&& other) : m_pipe(other.m_pipe) { other.m_pipe = nullptr; }

    Popen(const Popen&) = delete;

    ~Popen() { close(); }

    Popen& operator=(Popen&& other) {
        if (this != &other) {
            close();
            m_pipe       = other.m_pipe;
            other.m_pipe = nullptr;
        }
        return *this;
    }

    Popen& operator=(const Popen&) = delete;

    FILE* get() const { return m_pipe; }

    void close();

private:
    FILE* m_pipe = nullptr;
};


} // namespace allay_launcher::util::internal
