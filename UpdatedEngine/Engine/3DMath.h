#pragma once
#include "EngineCore.h"
#include <array>
#include <cmath>    // For sqrtf, acosf
#include <cstdint>

namespace ENGINE
{
    struct ENGINE_API float2
    {
        float x;
        float y;

        inline float& operator[](int i) { return (&x)[i]; }
        inline float operator[](int i) const { return (&x)[i]; }

        inline float* data() { return &x; }
        inline const float* data() const { return &x; }
        inline static constexpr size_t size() { return 2; }

        float2& operator=(const float2& that)
        {
            if (this != &that)
            {
                x = that.x;
                y = that.y;
            }
            return *this;
        }

        inline float length() const { return sqrtf(x * x + y * y); }
        inline float2 normalize() const
        {
            float len = length();
            if (len == 0.0f) return { 0.0f, 0.0f };
            return { x / len, y / len };
        }
    };

    struct ENGINE_API float3
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
            };

            float2 xy;
        };


        float3() : x(0), y(0), z(0) {};
        float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};

        float3& operator=(const float3& that)
        {
            if (this != &that)
            {
                x = that.x;
                y = that.y;
                z = that.z;
            }
            return *this;
        }

        inline float& operator[](int i) { return (&x)[i]; }
        inline float operator[](int i) const { return (&x)[i]; }

        inline float* data() { return &x; }
        inline const float* data() const { return &x; }
        inline static constexpr size_t size() { return 3; }
        inline static float3 cross(const float3& lhs, const float3& rhs);
        inline static float  dot(const float3& lhs, const float3& rhs);
        inline friend float3 operator+(float3 lhs, float3 rhs)
        {
            return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
        }

        inline friend float3 operator-(float3 lhs, float3 rhs)
        {
            return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
        }

        inline friend float3 operator*(float3 lhs, float3 rhs)
        {
            return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
        }

        inline friend float3 operator*(float3 lhs, float rhs)
        {
            return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
        }

        inline friend float3 operator/(float3 lhs, float3 rhs)
        {
            return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
        }

        inline friend float3 operator/(float3 lhs, float rhs)
        {
            return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
        }

        inline float dot(const float3& other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }

        inline float3 cross(const float3& other) const
        {
            return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
        }
        
        inline float length() const
        {
            return sqrtf(x * x + y * y + z * z);
        }

        inline float3 normalize() const
        {
            float length = sqrtf(x * x + y * y + z * z);
            if (length == 0.0f) return { 0.0f, 0.0f, 0.0f };
            return { x / length, y / length, z / length };
        }

        inline float distance(const float3& other) const
        {
            return sqrtf((x - other.x) * (x - other.x) +
                (y - other.y) * (y - other.y) +
                (z - other.z) * (z - other.z));
        }

        inline float angle(const float3& other) const
        {
            float dotProd = dot(other);
            float lengths = length() * other.length();
            return acosf(dotProd / lengths);
        }

        inline friend float3 lerp(const float3& a, const float3& b, float t)
        {
            return { a.x + t * (b.x - a.x), a.y + t * (b.y - a.y), a.z + t * (b.z - a.z) };
        }
         
        inline float3& operator+=(float3 rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;

            return *this;
        }

        inline float3& operator-=(float3 rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;

            return *this;
        }

        inline float3& operator*=(float3 rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;

            return *this;
        }

        inline float3& operator/=(float3 rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;

            return *this;
        }

        inline float3& operator*=(float rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;

            return *this;
        }

        inline float3& operator/=(float rhs)
        {
            x /= rhs;
            y /= rhs;
            z /= rhs;

            return *this;
        }
    }; //end of float3

    inline float3 float3::cross(const float3& lhs, const float3& rhs)
    {
        return {
            lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x };
    }

    inline float float3::dot(const float3& lhs, const float3& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }
    struct ENGINE_API float4
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };

            float3 xyz;

            struct
            {
                float2 xy;
                float2 zw;
            };
        };

        float4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
        float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

        inline float& operator[](int i) { return (&x)[i]; }
        inline float operator[](int i) const { return (&x)[i]; }

        inline float* data() { return &x; }
        inline const float* data() const { return &x; }
        inline static constexpr size_t size() { return 4; }

        float4& operator=(const float4& that)
        {
            if (this != &that)
            {
                x = that.x;
                y = that.y;
                z = that.z;
                w = that.w;
            }
            return *this;
        }

        inline friend float4 operator*(float4 lhs, float rhs)
        {
            return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
        }

        inline friend float4 operator/(float4 lhs, float rhs)
        {
            return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
        }

        inline float4& operator*=(float rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;

            return *this;
        }

        inline float4& operator/=(float rhs)
        {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            w /= rhs;

            return *this;
        }
    };

} // namespace ENGINE

