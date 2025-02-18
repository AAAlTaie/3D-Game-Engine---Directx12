#pragma once
#include "Math/float4x4.h"

namespace ENGINE
{
    class ENGINE_API Camera
    {
    public:
        Camera(float fov, float aspect, float z_near, float z_far);

        float4x4 get_projection();
        float4x4 get_view();
        void update_camera(float delta);

        void set_position(const float3& pos);

    private:

        bool m_dirty_flag = false;

        float m_fovy;
        float m_zear;
        float m_far;
        float m_aspect;

        float3 m_pos;
        float3 m_rot; 

        float m_pitch{0.0f}; //rot degree (x)
        float m_yaw{0.0f};   //rot degree (y)

        float3 m_right;
        float3 m_forward;
        float3 m_up;



        float4x4 m_projection;
        float4x4 m_view;  // Inverse of local
        float4x4 m_local;

        //TODO : internal update ---(view_matrix, RUF, (pitch, yaw))
        void internal_update();

    };
}