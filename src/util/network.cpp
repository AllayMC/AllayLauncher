#include <cpr/cpr.h>

#include "util/string.h"
#include "util/terminal.h"

#include "theme.h"

namespace allay_launcher::util::network {

void download(std::string_view download_url, std::string_view save_path) {
    using namespace fmt;

    logging::debug("download(): {}", download_url);

    std::ofstream save_file(save_path.data(), std::ios::binary | std::ios::trunc);
    if (!save_file) {
        throw IOException(save_path, "unable to open file.");
    }

    auto session = global::CreateCprSession();
    session->SetUrl(download_url);

    auto response = cpr::Response();
    auto host     = string::extract_host(download_url);

    // clang-format off
    terminal::Spinner()
        .printer([&](std::string_view spinner, std::string_view hint) {
            fmt::print(
                "\r  {} fetching from ... {}", 
                spinner, 
                fmt::styled(host, fg(rgb(theme::EMPHASIS)) | fmt::emphasis::bold)
            );
        })
        .printer_end([&](std::string_view spinner, std::string_view hint) {
            fmt::print("\r");
            if (response.status_code != 200) {
                logging::error("Download failed with status code: {}", response.status_code);
                std::filesystem::remove(save_path);
                throw ConnectionException(
                    (int)response.error.code,
                    response.status_code,
                    response.error.message,
                    response.url.str()
                );
            }
        })
        .scope([&]() {
            response = session->Download(save_file);
        })
        .run();
    // clang-format on
}


} // namespace allay_launcher::util::network
