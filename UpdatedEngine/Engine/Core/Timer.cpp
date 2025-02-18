#include "pch.h"
#include "Timer.h"
#include <chrono>

std::chrono::time_point<std::chrono::high_resolution_clock> current_time{};
std::chrono::time_point<std::chrono::high_resolution_clock> last_time{};

uint  ENGINE::Timer::frame_count = 0;
uint  ENGINE::Timer::frame_per_second = 0;
float ENGINE::Timer::delta_time = 0.f;

namespace ENGINE
{
    void Timer::init_frame()
    {
        current_time = std::chrono::high_resolution_clock::now();
        last_time = current_time;
    }

    void Timer::update_frame()
    {
        frame_count++;

        auto current = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> delta = current - last_time;
        last_time = current;

        delta_time = static_cast<float>(delta.count());

        auto duration = std::chrono::duration_cast<std::chrono::seconds>(
            current - current_time
        );

        if (duration.count() >= 1) {
            frame_per_second = frame_count;
            frame_count = 0;
            current_time = current;
        }
    }
}