#include "pch.h"
#include "Camera.h"
#include "Core/CoreInput.h"

namespace ENGINE
{
    template <typename T>
    T clamp(T value, T min_value, T max_value) {
        if (value < min_value) {
            return min_value;
        }
        if (value > max_value) {
            return max_value;
        }
        return value;
    }

    Camera::Camera(float fov, float aspect, float z_near, float z_far)
        : 
        m_fovy(fov), 
        m_aspect(aspect), 
        m_zear(z_near), 
        m_far(z_far)
    {
        m_projection = float4x4::perspective_left_hand(m_fovy, m_aspect, m_zear, m_far);

        m_right   = { 1.0f, 0.0f, 0.0f };
        m_up      = { 0.0f, 1.0f, 0.0f };
        m_forward = { 0.0f, 0.0f, 1.0f };

        m_dirty_flag = true;
        internal_update();

    }

    void Camera::update_camera(float delta)
    {
        const float movement_speed = 25.0f;
        const float rotation_sensitivity = 0.005f;

        // Rotation handling
        if (CoreInput::is_rotating_camera())
        {
            m_yaw += CoreInput::delta[0] * rotation_sensitivity;
            m_pitch += CoreInput::delta[1] * rotation_sensitivity;

            const float max_pitch = DirectX::XM_PI / 2.0f - 0.01f; // Slightly less than 90 degrees
            m_pitch = clamp(m_pitch, -max_pitch, max_pitch);
        }
     
        float4x4 R = float4x4::rotation_yaw_pitch_roll(m_yaw, m_pitch, 0.0f);

        m_right   = { R[0][0], R[1][0], R[2][0] }; m_right.normalize();
        m_up      = { R[0][1], R[1][1], R[2][1] }; m_up.normalize();
        m_forward = { R[0][2], R[1][2], R[2][2] }; m_forward.normalize();

        if (CoreInput::is_moving_forward())
        {
            m_pos += m_forward * movement_speed * delta;
        }
        if (CoreInput::is_moving_backward())
        {
            m_pos -= m_forward * movement_speed * delta;
        }
        if (CoreInput::is_moving_left())
        {
            m_pos -= m_right * movement_speed * delta;
        }
        if (CoreInput::is_moving_right())
        {
            m_pos += m_right * movement_speed * delta;
        }

        m_dirty_flag = true;
        internal_update();

    }

    void Camera::set_position(const float3& pos)
    {
        m_pos = pos;
        m_dirty_flag = false;
    }

    void Camera::internal_update()
    {
        if (!m_dirty_flag) return;

        m_local = {
            m_right.x, m_up.x, m_forward.x, m_pos.x,
            m_right.y, m_up.y, m_forward.y, m_pos.y,
            m_right.z, m_up.z, m_forward.z, m_pos.z,
            0.f, 0.f, 0.f, 1.f
        };

        float x = -float3::dot(m_right, m_pos);
        float y = -float3::dot(m_up, m_pos);
        float z = -float3::dot(m_forward, m_pos);

        m_view = {
            m_right.x,    m_right.y,    m_right.z,   x,
               m_up.x,       m_up.y,       m_up.z,   y,
          m_forward.x,  m_forward.y,  m_forward.z,   z,
               0.0f,       0.0f,       0.0f, 1.0f
        };

    }

    float4x4 Camera::get_projection()
    {
        return m_projection;
    }

    float4x4 Camera::get_view()
    {
        return m_view;
    }
}
