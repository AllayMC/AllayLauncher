#pragma once

namespace allay_launcher::util::file {

bool clean_and_write_file(const std::string& filename, const std::string& text);

std::string read_file(const std::string&& filename);

} // namespace allay_launcher::util::file