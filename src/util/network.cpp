#include "spdlog/spdlog.h"

#include <cpr/cpr.h>
#include <util/progress_bar.h>

namespace allay_launcher::util::network {

void download(std::string_view download_url, std::string_view save_path) {

    std::ofstream save_file(save_path.data(), std::ios::binary | std::ios::trunc);
    if (!save_file) {
        throw IOException(save_path, "unable to open file.");
    }

    logging::debug("download(): {}", download_url);

    progresscpp::ProgressBar progress_bar(100, 70);

    auto session = global::CreateCprSession();
    session->SetUrl(download_url);
    session->SetProgressCallback(cpr::ProgressCallback(
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
    auto response = session->Download(save_file);
    progress_bar.done(format(fg(fmt::color::green), "\u221a"));
    if (response.status_code != 200) {
        logging::error("Unable to download file. Status code: {}", response.status_code);
        std::filesystem::remove(save_path);
        throw ConnectionException(
            (int)response.error.code,
            response.status_code,
            response.error.message,
            response.url.str()
        );
    }
}


} // namespace allay_launcher::util::network
