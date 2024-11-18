#pragma once

namespace allay_launcher::util::network {

std::expected<void, DownloadFileError> download(std::string_view download_url, std::string_view save_path);

} // namespace allay_launcher::util::network