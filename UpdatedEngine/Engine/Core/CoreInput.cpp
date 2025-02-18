#include "pch.h"
#include "CoreInput.h"

namespace ENGINE
{
    float CoreInput::current_pos[2]{};
    float CoreInput::last_pos[2]{};
    float CoreInput::delta[2]{};

    HWND CoreInput::m_hwnd{ nullptr };
    bool CoreInput::key_state[256]{};
    bool CoreInput::prev_key_state[256]{};

    void CoreInput::init_frame(HWND main_hwnd)
    {
        m_hwnd = main_hwnd;
    }

    void CoreInput::update_frame()
    {
        // Update previous key states
        memcpy(prev_key_state, key_state, sizeof(key_state));

        // Update current key states
        for (int i = 0; i < 256; ++i) {
            key_state[i] = ::GetAsyncKeyState(i) & 0x8000;
        }

        // Update mouse position
        POINT pt{};
        ::GetCursorPos(&pt);
        if (m_hwnd) {
            ::ScreenToClient(m_hwnd, &pt);
        }
        current_pos[0] = static_cast<float>(pt.x);
        current_pos[1] = static_cast<float>(pt.y);

        // Calculate mouse delta for camera rotation
        if (is_rotating_camera()) {
            delta[0] = current_pos[0] - last_pos[0];
            delta[1] = current_pos[1] - last_pos[1];
        }
        else {
            delta[0] = 0.f;
            delta[1] = 0.f;
        }

        // Update last mouse position
        memcpy(last_pos, current_pos, sizeof(float) * 2);
    }

    bool CoreInput::is_key_down(Key key)
    {
        return key_state[static_cast<int>(key)];
    }

    bool CoreInput::is_key_pressed(Key key)
    {
        int code = static_cast<int>(key);
        return key_state[code] && !prev_key_state[code];
    }

    bool CoreInput::is_key_released(Key key)
    {
        int code = static_cast<int>(key);
        return !key_state[code] && prev_key_state[code];
    }

    bool CoreInput::is_moving_forward()
    {
        return is_key_down(Key::W);
    }

    bool CoreInput::is_moving_backward()
    {
        return is_key_down(Key::S);
    }

    bool CoreInput::is_moving_left()
    {
        return is_key_down(Key::A);
    }

    bool CoreInput::is_moving_right()
    {
        return is_key_down(Key::D);
    }

    bool CoreInput::is_rotating_camera()
    {
        return is_key_down(Key::RMB);
    }
}
