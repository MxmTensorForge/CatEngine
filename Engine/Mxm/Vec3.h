#ifndef VEC3_H
#define VEC3_H

#include <cmath>

#include "Vec2.h"
#include "Consts.h"

namespace Mxm
{
	struct Vec3 {
		float x, y, z;

		Vec3() noexcept;
		explicit Vec3(float s) noexcept;
		explicit Vec3(float vx, float vy, float vz) noexcept;
		explicit Vec3(const Vec2& vec, float vz) noexcept;

		float& operator[](size_t index) noexcept;
		const float& operator[](size_t index) const noexcept;

		Vec3 operator+(const Vec3& vec) const noexcept;
		Vec3 operator-(const Vec3& vec) const noexcept;
		Vec3 operator*(const Vec3& vec) const noexcept;
		Vec3 operator/(const Vec3& vec) const noexcept;

		Vec3& operator+=(const Vec3& vec) noexcept;
		Vec3& operator-=(const Vec3& vec) noexcept;
		Vec3& operator*=(const Vec3& vec) noexcept;
		Vec3& operator/=(const Vec3& vec) noexcept;

		Vec3 operator+(float s) const noexcept;
		Vec3 operator-(float s) const noexcept;
		Vec3 operator*(float s) const noexcept;
		Vec3 operator/(float s) const noexcept;
		Vec3& operator+=(float s) noexcept;
		Vec3& operator-=(float s) noexcept;
		Vec3& operator*=(float s) noexcept;
		Vec3& operator/=(float s) noexcept;

		Vec3 operator-() const noexcept;

		bool operator==(const Vec3& vec) const noexcept;
		bool operator!=(const Vec3& vec) const noexcept;

		float dot(const Vec3& vec) const noexcept;
		Vec3 cross(const Vec3& vec) const noexcept;
		float length() const noexcept;
		Vec3 normalized() const noexcept;

		Vec3 reflect(const Vec3& normal) const noexcept;
		float angleBetween(const Vec3& vec) const noexcept;
		Vec3 lerp(const Vec3& vec, float t) const noexcept;
		Vec3 slerp(const Vec3& vec, float t) const noexcept;
	};
}

#include "src/Vec3.inl"

#endif // !MXM_H
