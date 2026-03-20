namespace Mxm
{
	inline Quat::Quat() noexcept : _w{}, _x{}, _y{}, _z{} {}
	inline Quat::Quat(float w, float x, float y, float z) noexcept 
		: _w{w}, _x{x}, _y{y}, _z{z} {}

	inline float Quat::w() const noexcept { return _w; }
	inline float Quat::x() const noexcept { return _x; }
	inline float Quat::y() const noexcept { return _y; }
	inline float Quat::z() const noexcept { return _z; }

	inline Quat Quat::operator*(const Quat& other) const noexcept {
		return Quat(
			_w * other._w - _x * other._x - _y * other._y - _z * other._z,
			_w * other._x + _x * other._w + _y * other._z - _z * other._y,
			_w * other._y + _x * other._z + _y * other._w - _z * other._x,
			_w * other._z + _x * other._y - _y * other._x + _z * other._w
		);
	}
	inline Quat Quat::operator+(const Quat& other) const noexcept {
		return Quat(
			_w + other._w, _x + other._x, _y + other._y, _z + other._z
		);
	}
	inline Quat Quat::operator-(const Quat& other) const noexcept {
		return Quat(
			_w - other._w, _x - other._x, _y - other._y, _z - other._z
		);
	}

	inline Quat& Quat::operator*=(const Quat& other) noexcept {
		*this = *this * other;
		return *this;
	}
	inline Quat& Quat::operator+=(const Quat& other) noexcept {
		*this = *this + other;
		return *this;
	}
	inline Quat& Quat::operator-=(const Quat& other) noexcept {
		*this = *this - other;
		return *this;
	}

	inline Quat Quat::operator*(float s) const noexcept {
		return Quat(
			_w * s, _x * s, _y * s, _z * s
		);
	}
	inline Quat Quat::operator/(float s) const noexcept {
		return Quat(
			_w / s, _x / s, _y / s, _z / s
		);
	}
	inline Quat Quat::operator+(float s) const noexcept {
		return Quat(
			_w + s, _x + s, _y + s, _z + s
		);
	}
	inline Quat Quat::operator-(float s) const noexcept {
		return Quat(
			_w - s, _x - s, _y - s, _z - s
		);
	}

	inline Quat& Quat::operator*=(float s) noexcept {
		*this = *this * s;
		return *this;
	}
	inline Quat& Quat::operator/=(float s) noexcept {
		*this = *this / s;
		return *this;
	}
	inline Quat& Quat::operator+=(float s) noexcept {
		*this = *this + s;
		return *this;
	}
	inline Quat& Quat::operator-=(float s) noexcept {
		*this = *this - s;
		return *this;
	}

	inline float Quat::length2() const noexcept {
		return _w * _w + _x * _x + _y * _y + _z * _z;
	}
	inline float Quat::length() const noexcept {
		return sqrtf(length2());
	}

	inline Quat Quat::conjucated() const noexcept {
		return Quat(_w, -_x, -_y, -_z);
	}
	inline Quat Quat::inversed() const noexcept {
		return conjucated() / length2();
	}

	inline Quat Quat::euler(float pitch, float yaw, float roll) noexcept {
		float halfPitchRad = Mxm::Consts::DEG2RAD * pitch * 0.5f;
		float halfYawRad = Mxm::Consts::DEG2RAD * yaw * 0.5f;
		float halfRollRad = Mxm::Consts::DEG2RAD * roll * 0.5f;

		Quat qPitch = Quat(cosf(halfPitchRad), sinf(halfPitchRad), 0.0f, 0.0f);
		Quat qYaw = Quat(cosf(halfYawRad), 0.0f, sinf(halfYawRad), 0.0f);
		Quat qRoll = Quat(cosf(halfRollRad), 0.0f, 0.0f, sinf(halfRollRad));

		return qYaw * qPitch * qRoll;
	}
	inline Quat Quat::aroundAxis(const Mxm::Vec3& axis, float angle) noexcept {
		float halfAngleRad = Mxm::Consts::DEG2RAD * angle * 0.5f;

		float c = cosf(halfAngleRad), s = sinf(halfAngleRad);
		return Quat(c, s * axis.x, s * axis.y, s * axis.z);
	}
}