#ifndef QUAT_H
#define QUAT_H

#include <cmath>
#include "Vec3.h"

namespace Mxm
{
	class Quat final
	{
	private:
		float _w, _x, _y, _z;
	public:
		Quat() noexcept;
		Quat(float w, float x, float y, float z) noexcept;

		float w() const noexcept;
		float x() const noexcept;
		float y() const noexcept;
		float z() const noexcept;

		Quat operator*(const Quat& other) const noexcept;
		Quat operator+(const Quat& other) const noexcept;
		Quat operator-(const Quat& other) const noexcept;

		Quat& operator*=(const Quat& other) noexcept;
		Quat& operator+=(const Quat& other) noexcept;
		Quat& operator-=(const Quat& other) noexcept;

		Quat operator*(float s) const noexcept;
		Quat operator/(float s) const noexcept;
		Quat operator+(float s) const noexcept;
		Quat operator-(float s) const noexcept;

		Quat& operator*=(float s) noexcept;
		Quat& operator/=(float s) noexcept;
		Quat& operator+=(float s) noexcept;
		Quat& operator-=(float s) noexcept;

		float length2() const noexcept;
		float length() const noexcept;

		Quat conjucated() const noexcept;
		Quat inversed() const noexcept;

		static Quat euler(float pitch, float yaw, float roll) noexcept;
		static Quat aroundAxis(const Mxm::Vec3& axis, float angle) noexcept;
	};
}

#include "src/Quat.inl"

#endif // !QUAT_H
