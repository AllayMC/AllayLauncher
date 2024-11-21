#include "util/network.h"

#include <cpr/cpr.h>
#include <util/progress_bar.h>

namespace allay_launcher::util::network {

void download(cpr::Session& session, std::string_view download_url, std::string_view save_path) {
    if (std::filesystem::exists(save_path)) {
        throw DownloadFileException::FileExistsError();
    }

    std::ofstream save_file(save_path.data(), std::ios::binary);
    if (!save_file) {
        throw DownloadFileException::UnableToOpenFileError();
    }

    progresscpp::ProgressBar progress_bar(100, 70);
    session.SetUrl(download_url);
    session.SetProgressCallback(cpr::ProgressCallback(
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
    ));
    auto download_response = session.Download(save_file);
    progress_bar.done(format(fg(fmt::color::green), "\u221a"));
    if (download_response.status_code != 200) {
        logging::error("Unable to download file. Status code: {}", download_response.status_code);
        std::filesystem::remove(save_path);
        throw DownloadFileException::NetworkError();
    }
}


} // namespace allay_launcher::util::network
