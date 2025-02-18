#pragma once

namespace ENGINE {


    class ENGINE_API Timer
    {
    public:
        Timer() = default;
        Timer(const Timer& rhs) = delete;
        Timer& operator=(const Timer& rhs) = delete;

        static void init_frame();
        static void update_frame();

        static uint frame_count;
        static uint frame_per_second;
        static float delta_time;

    };

}