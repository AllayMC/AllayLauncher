#pragma once

#include <iostream>
#include <spdlog/fmt/bundled/color.h>

namespace progresscpp {
class ProgressBar {
private:
    unsigned int ticks = 0;

    const unsigned int total_ticks;
    const unsigned int bar_width;
    const std::string  complete_str   = format(fg(spdlog::fmt_lib::color::green), "\u2501");
    const std::string  incomplete_str = format(fg(spdlog::fmt_lib::color::gray), "\u2501");

public:
    ProgressBar(unsigned int total, unsigned int width, std::string complete, std::string incomplete)
    : total_ticks{total},
      bar_width{width},
      complete_str{complete},
      incomplete_str{incomplete} {}

    ProgressBar(unsigned int total, unsigned int width) : total_ticks{total}, bar_width{width} {}

    unsigned int operator++() { return ++ticks; }

    void set_ticks(unsigned int ticks) { this->ticks = ticks; }

    void set_progress(float progress) { this->ticks = total_ticks * progress; }

    void display(std::string extra_text = "") const {
        float progress = (float)ticks / total_ticks;
        int   pos      = (int)(bar_width * progress);

        std::cout << int(progress * 100.0) << "% ";

        for (int i = 0; i < bar_width; ++i) {
            if (i <= pos) std::cout << complete_str;
            else std::cout << incomplete_str;
        }
        // Use some blank before \r to make sure that the new
        // characters can overwrite the old characters fully
        std::cout << " " << extra_text << "          \r";
        std::cout.flush();
    }

    void done(std::string extra_text = "") const {
        display(extra_text);
        std::cout << std::endl;
    }
};
} // namespace progresscpp
