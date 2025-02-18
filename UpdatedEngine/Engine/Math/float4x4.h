#pragma once
#include <iostream>
#include <iomanip>

#define M_PI			3.14159265358979323846f
#define radian		    (M_PI / 180.f)

namespace ENGINE
{
	struct ENGINE_API float4x4
	{
		union
		{
			float f[16];
			float m[4][4];
		};

		float4x4();
		float4x4(float* rhs);
		float4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		);
		
		inline void indentity();

		static float4x4 identity_static();

		float* operator[](unsigned int i);
		const float* operator[](unsigned int i) const;

		float4x4 operator*(const float4x4& rhs);

		//func
		float4x4 inverted();


		//static
		static float4x4 rotation_x(float angle);
		static float4x4 rotation_y(float angle);

		static float4x4 rotation_yaw_pitch_roll(float yaw, float pitch, float roll);

		static float4x4 perspective_left_hand(
			float fov,
			float ratio,
			float znear,
			float zfar);


		//static float4x4 create_rotation_Z(float angle);
		static float4x4 create_translate(float x, float y, float z);




	};

	inline float4x4::float4x4() {
		indentity();
	}

	inline float4x4::float4x4(float* rhs) {
		memcpy_s(f, sizeof(f), rhs, sizeof(f));
	}

	inline float4x4::float4x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03;
		m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13;
		m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23;
		m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;
	}

	inline void float4x4::indentity()
	{
		memset(f, 0, sizeof(float4x4));
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
	}

	// Static identity method (can be used as `float4x4::identity_static()`)
	inline float4x4 float4x4::identity_static() {
		return float4x4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
	}

	inline float* float4x4::operator[](unsigned int i)
	{
		return m[i];
	}

	inline const float* float4x4::operator[](unsigned int i) const
	{
		return m[i];
	}

	inline float4x4 float4x4::operator*(const float4x4& rhs)
	{
		float4x4 mat{};

		for (int row = 0; row < 4; row++) {

			for (int col = 0; col < 4; col++) {
				mat[row][col] =
					m[row][0] * rhs[0][col] +
					m[row][1] * rhs[1][col] +
					m[row][2] * rhs[2][col] +
					m[row][3] * rhs[3][col];
			}
		}

		return mat;
	}

	inline float4x4 float4x4::inverted()
	{
		float inv[16] = {};
		float det = 0.0f;

		inv[0] =  f[5]  * f[10] * f[15] -
			      f[5]  * f[11] * f[14] -
			      f[9]  * f[6]  * f[15] +
			      f[9]  * f[7]  * f[14] +
			      f[13] * f[6]  * f[11] -
			      f[13] * f[7]  * f[10];

		inv[4] = -f[4]  * f[10] * f[15] +
			      f[4]  * f[11] * f[14] +
			      f[8]  * f[6]  * f[15] -
			      f[8]  * f[7]  * f[14] -
			      f[12] * f[6]  * f[11] +
			      f[12] * f[7]  * f[10];

		inv[8] =  f[4]  * f[9]  * f[15] -
			      f[4]  * f[11] * f[13] -
			      f[8]  * f[5]  * f[15] +
			      f[8]  * f[7]  * f[13] +
			      f[12] * f[5]  * f[11] -
			      f[12] * f[7]  * f[9];

		inv[12] = -f[4]  * f[9]  * f[14] +
			       f[4]  * f[10] * f[13] +
			       f[8]  * f[5]  * f[14] -
			       f[8]  * f[6]  * f[13] -
			       f[12] * f[5]  * f[10] +
			       f[12] * f[6]  * f[9];

		inv[1] = -f[1]  * f[10] * f[15] +
			      f[1]  * f[11] * f[14] +
			      f[9]  * f[2]  * f[15] -
			      f[9]  * f[3]  * f[14] -
			      f[13] * f[2]  * f[11] +
			      f[13] * f[3]  * f[10];

		inv[5] =  f[0]  * f[10] * f[15] -
			      f[0]  * f[11] * f[14] -
			      f[8]  * f[2]  * f[15] +
			      f[8]  * f[3]  * f[14] +
			      f[12] * f[2]  * f[11] -
			      f[12] * f[3]  * f[10];

		inv[9] = -f[0] * f[9] * f[15] +
			f[0] * f[11] * f[13] +
			f[8] * f[1] * f[15] -
			f[8] * f[3] * f[13] -
			f[12] * f[1] * f[11] +
			f[12] * f[3] * f[9];

		inv[13] = f[0] * f[9] * f[14] -
			f[0] * f[10] * f[13] -
			f[8] * f[1] * f[14] +
			f[8] * f[2] * f[13] +
			f[12] * f[1] * f[10] -
			f[12] * f[2] * f[9];

		inv[2] = f[1] * f[6] * f[15] -
			f[1] * f[7] * f[14] -
			f[5] * f[2] * f[15] +
			f[5] * f[3] * f[14] +
			f[13] * f[2] * f[7] -
			f[13] * f[3] * f[6];

		inv[6] = -f[0] * f[6] * f[15] +
			f[0] * f[7] * f[14] +
			f[4] * f[2] * f[15] -
			f[4] * f[3] * f[14] -
			f[12] * f[2] * f[7] +
			f[12] * f[3] * f[6];

		inv[10] = f[0] * f[5] * f[15] -
			f[0] * f[7] * f[13] -
			f[4] * f[1] * f[15] +
			f[4] * f[3] * f[13] +
			f[12] * f[1] * f[7] -
			f[12] * f[3] * f[5];

		inv[14] = -f[0] * f[5] * f[14] +
			f[0] * f[6] * f[13] +
			f[4] * f[1] * f[14] -
			f[4] * f[2] * f[13] -
			f[12] * f[1] * f[6] +
			f[12] * f[2] * f[5];

		inv[3] = -f[1] * f[6] * f[11] +
			f[1] * f[7] * f[10] +
			f[5] * f[2] * f[11] -
			f[5] * f[3] * f[10] -
			f[9] * f[2] * f[7] +
			f[9] * f[3] * f[6];

		inv[7] = f[0] * f[6] * f[11] -
			f[0] * f[7] * f[10] -
			f[4] * f[2] * f[11] +
			f[4] * f[3] * f[10] +
			f[8] * f[2] * f[7] -
			f[8] * f[3] * f[6];

		inv[11] = -f[0] * f[5] * f[11] +
			f[0] * f[7] * f[9] +
			f[4] * f[1] * f[11] -
			f[4] * f[3] * f[9] -
			f[8] * f[1] * f[7] +
			f[8] * f[3] * f[5];

		inv[15] = f[0] * f[5] * f[10] -
			f[0] * f[6] * f[9] -
			f[4] * f[1] * f[10] +
			f[4] * f[2] * f[9] +
			f[8] * f[1] * f[6] -
			f[8] * f[2] * f[5];

		det = f[0] * inv[0] + f[1] * inv[4] + f[2] * inv[8] + f[3] * inv[12];

		if (det == 0)
			return float4x4();

		det = 1.0f / det;

		float4x4 mat{};
		for (int i = 0; i < 16; i++)
			mat.f[i] = inv[i] * det;

		return mat;
	}

	inline float4x4 float4x4::rotation_x(float angle)
	{
		float theta = radian * angle;
		float cos_theta = cos(theta);
		float sin_theta = sin(theta);

		return float4x4(
			1, 0, 0, 0,
			0, cos_theta, -sin_theta, 0,
			0, sin_theta, cos_theta, 0,
			0, 0, 0, 1
		);
	}

	inline float4x4 float4x4::rotation_y(float angle)
	{
		float theta = radian * angle;
		float cos_theta = cos(theta);
		float sin_theta = sin(theta);

		return float4x4(
			cos_theta, 0,  sin_theta, 0,
			0, 0, 0, 0,
			-sin_theta, 0, cos_theta, 0,
			0, 0, 0, 1
		);
	}

	inline float4x4 float4x4::rotation_yaw_pitch_roll(float yaw, float pitch, float roll)
	{
		// Calculate trigonometric values for yaw, pitch, and roll
		float cos_y = std::cosf(yaw);
		float sin_y = std::sinf(yaw);

		float cos_p = std::cosf(pitch);
		float sin_p = std::sinf(pitch);

		float cos_r = std::cosf(roll);
		float sin_r = std::sinf(roll);

		float m00 = cos_y * cos_r + sin_y * sin_p * sin_r;
		float m01 = cos_p * sin_r;
		float m02 = -sin_y * cos_r + cos_y * sin_p * sin_r;
		float m03 = 0.0f;

		float m10 = -cos_y * sin_r + sin_y * sin_p * cos_r;
		float m11 = cos_p * cos_r;
		float m12 = sin_y * sin_r + cos_y * sin_p * cos_r;
		float m13 = 0.0f;

		float m20 = sin_y * cos_p;
		float m21 = -sin_p;
		float m22 = cos_y * cos_p;
		float m23 = 0.0f;

		float m30 = 0.0f;
		float m31 = 0.0f;
		float m32 = 0.0f;
		float m33 = 1.0f;

		return float4x4(
			m00, m10, m20, m30,
			m01, m11, m21, m31,
			m02, m12, m22, m32,
			m03, m13, m23, m33);
	}

	inline float4x4 float4x4::perspective_left_hand(float fov, float ratio, float znear, float zfar)
	{
		float half_fov = fov * 0.5f;
		const float tan_fov = std::tanf(half_fov * radian);

		float A = 1.f / (ratio * tan_fov);
		float B = 1.0f / tan_fov;
		float C = zfar / (zfar - znear);
		float D = (-znear * zfar) / (zfar - znear);
		float E = 1.0f;

		auto mat = float4x4(
			A, 0.0f, 0.0f, 0.f,
			0.f, B, 0.0f, 0.f,
			0.f, 0.0f, C, D,
			0.f, 0.0f, E, 0.f
		);

		return mat;
	}

	inline float4x4 float4x4::create_translate(float x, float y, float z) {
		return {
			1.f, 0.f, 0.f, x,
			0.f, 1.f, 0.f, y,
			0.f, 0.f, 1.f, z,
			0.f, 0.f, 0.f, 1.f
		};
	}

	inline std::ostream& operator<<(std::ostream& os, const float4x4& mat) {
#define fm std::setw(12)
		os << "float4x4[" << "\n" <<
			fm << mat[0][0] << fm << mat[0][1] << fm << mat[0][2] << fm << mat[0][3] << "\n" <<
			fm << mat[1][0] << fm << mat[1][1] << fm << mat[1][2] << fm << mat[1][3] << "\n" <<
			fm << mat[2][0] << fm << mat[2][1] << fm << mat[2][2] << fm << mat[2][3] << "\n" <<
			fm << mat[3][0] << fm << mat[3][1] << fm << mat[3][2] << fm << mat[3][3] << " ]" << "\n";
#undef fm
		return os;
	}

}