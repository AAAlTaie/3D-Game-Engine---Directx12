#pragma once
#include <array>
#include <cmath>    // For sqrtf, acosf
#include <cstdint>
#include <pch.h>

namespace ENGINE
{
    struct float2
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

    struct float3
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

        float3() : x(0), y(0), z(0) {}
        float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

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
    };

    struct float4
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

    struct float4x4
    {
        std::array<float4, 4> rows;

        inline float4x4 operator*(const float4x4& other) const
        {
            float4x4 result;
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    result.rows[i][j] = 0;
                    for (int k = 0; k < 4; ++k)
                    {
                        result.rows[i][j] += rows[i][k] * other.rows[k][j];
                    }
                }
            }
            return result;
        }

        inline float4x4& operator*=(const float4x4& other)
        {
            *this = *this * other;
            return *this;
        }
    };

    struct alignas(8) float2_a : float2 {};

    struct alignas(16) float3_a : float3 {};

    struct alignas(16) float4_a : float4 {};

    using float4x4_a = std::array< float4_a, 4 >;

    struct colored_vertex
    {
        float3 pos = { 0.0f, 0.0f, 0.0f };
        float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        colored_vertex() = default;
        colored_vertex(const colored_vertex&) = default;

        inline colored_vertex(const float3& p, const float4& c) : pos{ p }, color{ c } {}
        inline colored_vertex(const float3& p, const float3& c) : pos{ p }, color{ c.x, c.y, c.z, 1.0f } {}
        inline colored_vertex(const float3& p, 
            uint8_t r, 
            uint8_t g, 
            uint8_t b, 
            uint8_t a = 255) : pos{ p }, color{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f } {}
    };

} // namespace ENGINE




//#pragma once
//#include <array>
//#include <cstdint>
//
//
//namespace ENGINE
//{
//	struct float2
//	{
//		float x;
//		float y;
//
//		inline float& operator[](int i) { return (&x)[i]; }
//		inline float operator[](int i)const { return (&x)[i]; }
//
//		inline float* data() { return &x; }
//		inline const float* data()const { return &x; }
//		inline static constexpr size_t size() { return 2; }
//
//		float2& operator=(const float2& that)
//		{
//			if (this != &that)
//			{
//				x = that.x;
//				y = that.y;
//			}
//			return *this;
//		}
//	};
//
//	struct float3
//	{
//		union
//		{
//			struct
//			{
//				float x;
//				float y;
//				float z;
//			};
//
//			float2 xy;
//		};
//
//		float3() : x(0), y(0), z(0) {}
//		float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
//
//		float3& operator=(const float3& that)
//		{
//			if (this != &that)
//			{
//				x = that.x;
//				y = that.y;
//				z = that.z;
//			}
//			return *this;
//		}
//
//		inline float& operator[](int i) { return (&x)[i]; }
//		inline float operator[](int i)const { return (&x)[i]; }
//
//		inline float* data() { return &x; }
//		inline const float* data()const { return &x; }
//		inline static constexpr size_t size() { return 3; }
//
//		inline friend float3 operator+(float3 lhs, float3 rhs)
//		{
//			return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
//		}
//
//		inline friend float3 operator-(float3 lhs, float3 rhs)
//		{
//			return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
//		}
//
//		inline friend float3 operator*(float3 lhs, float3 rhs)
//		{
//			return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
//		}
//
//		inline friend float3 operator*(float3 lhs, float rhs)
//		{
//			return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
//		}
//
//		inline friend float3 operator/(float3 lhs, float3 rhs)
//		{
//			return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
//		}
//
//		inline friend float3 operator/(float3 lhs, float rhs)
//		{
//			return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
//		}
//
//		inline float dot(const float3& lhs, const float3& rhs) {
//			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
//		}
//
//		inline float3 cross(const float3& lhs, const float3& rhs) {
//			return { lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x };
//		}
//
//		inline float3 normalize(float3 n)
//		{
//			float length = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
//			if (length == 0.0f)
//			{
//				float3 value = { 0.0f, 0.0f, 0.0f };
//				return value;
//			}
//			float3 value = { n.x / length, n.y / length, n.z / length };
//			return value;
//		}
//		inline float3& operator+=(float3 rhs)
//		{
//			x += rhs.x;
//			y += rhs.y;
//			z += rhs.z;
//
//			return *this;
//		}
//
//		inline float3& operator-=(float3 rhs)
//		{
//			x -= rhs.x;
//			y -= rhs.y;
//			z -= rhs.z;
//
//			return *this;
//		}
//
//		inline float3& operator*=(float3 rhs)
//		{
//			x *= rhs.x;
//			y *= rhs.y;
//			z *= rhs.z;
//
//			return *this;
//		}
//
//		inline float3& operator/=(float3 rhs)
//		{
//			x /= rhs.x;
//			y /= rhs.y;
//			z /= rhs.z;
//
//			return *this;
//		}
//
//		inline float3& operator*=(float rhs)
//		{
//			x *= rhs;
//			y *= rhs;
//			z *= rhs;
//
//			return *this;
//		}
//
//		inline float3& operator/=(float rhs)
//		{
//			x /= rhs;
//			y /= rhs;
//			z /= rhs;
//
//			return *this;
//		}
//	};
//
//	struct float4
//	{
//		union
//		{
//			struct
//			{
//				float x;
//				float y;
//				float z;
//				float w;
//			};
//
//			float3 xyz;
//
//			struct
//			{
//				float2 xy;
//				float2 zw;
//			};
//		};
//
//		float4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
//		float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }
//
//		inline float& operator[](int i) { return (&x)[i]; }
//		inline float operator[](int i)const { return (&x)[i]; }
//
//		inline float* data() { return &x; }
//		inline const float* data()const { return &x; }
//		inline static constexpr size_t size() { return 4; }
//
//		float4& operator=(const float4& that)
//		{
//			if (this != &that)
//			{
//				x = that.x;
//				y = that.y;
//				z = that.z;
//				w = that.w;
//			}
//			return *this;
//		}
//
//		inline friend float4 operator*(float4 lhs, float rhs)
//		{
//			return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
//		}
//
//		inline friend float4 operator/(float4 lhs, float rhs)
//		{
//			return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
//		}
//
//		inline float4& operator*=(float rhs)
//		{
//			x *= rhs;
//			y *= rhs;
//			z *= rhs;
//			w *= rhs;
//
//			return *this;
//		}
//
//		inline float4& operator/=(float rhs)
//		{
//			x /= rhs;
//			y /= rhs;
//			z /= rhs;
//			w /= rhs;
//
//			return *this;
//		}
//	};
//
//	struct alignas(8) float2_a : float2 {};
//
//	struct alignas(16) float3_a : float3 {};
//
//	struct alignas(16) float4_a : float4 {};
//
//	using float4x4 = std::array< float4, 4 >;
//
//	using float4x4_a = std::array< float4_a, 4 >;
//
//}
//
//namespace ENGINE
//{
//	struct colored_vertex
//	{
//		float3 pos = { 0.0f, 0.0f, 0.0f };
//		float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
//
//		colored_vertex() = default;
//		colored_vertex(const colored_vertex&) = default;
//
//		inline colored_vertex(const float3& p, const float4& c) : pos{ p }, color{ c } {}
//		inline colored_vertex(const float3& p, const float3& c) : pos{ p }, color{ c.x, c.y, c.z, 1.0f } {}
//		inline colored_vertex(const float3& p, 
//			uint8_t r, 
//			uint8_t g, 
//			uint8_t b, 
//			uint8_t a = 255) : pos{ p }, color{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f } {}
//	};
//}