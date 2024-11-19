#include <filesystem>
#include <fstream>

#include "file.h"

namespace allay_launcher::util::file {

bool clean_and_write_file(const std::string& filename, const std::string& text) {
    std::ofstream file(filename, std::ios::trunc);
    if (file.is_open()) {
        file << text;
        file.close();
    } else {
        logging::error("Error opening file for writing.");
        return false;
    }
    return true;
}

std::string read_file(const std::string&& filename) {
    if (!std::filesystem::exists(filename)) {
        return "";
    }
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::string content;
        while (std::getline(file, line)) {
            if (!content.empty()) {
                content += '\n';
            }
            content += line;
        }
        file.close();
        return content;
    } else {
        return "";
    }
}

bool remove_if_exists(std::string_view filename) {
    if (std::filesystem::exists(filename)) {
        return std::filesystem::remove(filename);
    }
    return false;
}
} // namespace allay_launcher::util::file