#pragma once
#include <Windows.h>

namespace ENGINE
{
    enum class Key
    {
        LMB = 0x01,
        RMB = 0x02,
        MMB = 0x04,
        W = 0x57,
        A = 0x41,
        S = 0x53,
        D = 0x44
    };

    class CoreInput
    {
    public:
        CoreInput() = default;
        CoreInput(const CoreInput& rhs) = delete;
        CoreInput& operator=(const CoreInput& rhs) = delete;

        static void init_frame(HWND main_hwnd);
        static void update_frame();

        // Mouse
        static float current_pos[2];
        static float last_pos[2];
        static float delta[2];

        // Keyboard
        static bool is_key_down(Key key);
        static bool is_key_pressed(Key key);
        static bool is_key_released(Key key);

        // Camera Movement
        static bool is_moving_forward();
        static bool is_moving_backward();
        static bool is_moving_left();
        static bool is_moving_right();
        static bool is_rotating_camera();

    private:
        static HWND m_hwnd;
        static bool key_state[256];
        static bool prev_key_state[256];
    };
}