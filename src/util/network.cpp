#include "util/network.h"

#include <cpr/cpr.h>
#include <util/progress_bar.h>

namespace allay_launcher::util::network {

std::expected<void, DownloadFileError> download(std::string_view download_url, std::string_view save_path) {
    if (std::filesystem::exists(save_path)) {
        return std::unexpected(DownloadFileError::FileExistsError);
    }

    std::ofstream save_file(save_path.data(), std::ios::binary);
    if (!save_file) {
        return std::unexpected(DownloadFileError::UnableToOpenFileError);
    }

    progresscpp::ProgressBar progress_bar(100, 70);
    cpr::cpr_off_t           upload_last       = 0;
    auto                     download_response = cpr::Download(
        save_file,
        cpr::Url{download_url},
        cpr::ProgressCallback(
            [&progress_bar](
                cpr::cpr_off_t download_total,
                cpr::cpr_off_t download_now,
                cpr::cpr_off_t upload_total,
                cpr::cpr_off_t upload_now,
                intptr_t       userdata
            ) -> bool {
                progress_bar.set_progress((float)download_now / download_total);
                progress_bar.display(
                    fmt::format("{:.2}/{:.2} MB", download_now / (1024.0 * 1024.0), download_total / (1024.0 * 1024.0))
                );
                return true;
            }
        )
    );
    progress_bar.done(format(fg(fmt::color::green), "\u221a"));
    if (download_response.status_code != 200) {
        std::filesystem::remove(save_path);
        return std::unexpected(DownloadFileError::NetworkError);
    }
    return {};
}

} // namespace allay_launcher::util::network
