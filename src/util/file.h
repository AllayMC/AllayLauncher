#pragma once

#include <string_view>
namespace allay_launcher::util::file {

bool clean_and_write_file(const std::string& filename, const std::string& text);

std::string read_file(const std::string&& filename);

bool remove_if_exists(std::string_view filename);

} // namespace allay_launcher::util::file