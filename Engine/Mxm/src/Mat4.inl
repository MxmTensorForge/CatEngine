namespace Mxm
{
    inline Mat4::Mat4() noexcept : _m{ 0.0f } {}

    inline Mat4::Mat4(float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33) noexcept :
        _m{ {a00, a01, a02, a03},
             {a10, a11, a12, a13},
             {a20, a21, a22, a23},
             {a30, a31, a32, a33} } {
    }

    inline Mat4::Mat4(const Vec4& vec1, const Vec4& vec2, const Vec4& vec3, const Vec4& vec4) noexcept :
        _m{ {vec1.x, vec2.x, vec3.x, vec4.x},
             {vec1.y, vec2.y, vec3.y, vec4.y},
             {vec1.z, vec2.z, vec3.z, vec4.z},
             {vec1.w, vec2.w, vec3.w, vec4.w} } {
    }

    inline Vec4 Mat4::operator[](size_t index) const noexcept {
        if (index > 3) return Vec4();
        return Vec4(_m[index][0], _m[index][1], _m[index][2], _m[index][3]);
    }
    inline Vec4 Mat4::col(size_t index) const noexcept {
        if (index > 3) return Vec4();
        return Vec4(_m[0][index], _m[1][index], _m[2][index], _m[3][index]);
    }

    inline Mat4 Mat4::operator+(const Mat4& mat) const noexcept {
        return Mat4(_m[0][0] + mat._m[0][0], _m[0][1] + mat._m[0][1], _m[0][2] + mat._m[0][2], _m[0][3] + mat._m[0][3],
                    _m[1][0] + mat._m[1][0], _m[1][1] + mat._m[1][1], _m[1][2] + mat._m[1][2], _m[1][3] + mat._m[1][3],
                    _m[2][0] + mat._m[2][0], _m[2][1] + mat._m[2][1], _m[2][2] + mat._m[2][2], _m[2][3] + mat._m[2][3],
                    _m[3][0] + mat._m[3][0], _m[3][1] + mat._m[3][1], _m[3][2] + mat._m[3][2], _m[3][3] + mat._m[3][3]);
    }

    inline Mat4 Mat4::operator-(const Mat4& mat) const noexcept {
        return Mat4(_m[0][0] - mat._m[0][0], _m[0][1] - mat._m[0][1], _m[0][2] - mat._m[0][2], _m[0][3] - mat._m[0][3],
                    _m[1][0] - mat._m[1][0], _m[1][1] - mat._m[1][1], _m[1][2] - mat._m[1][2], _m[1][3] - mat._m[1][3],
                    _m[2][0] - mat._m[2][0], _m[2][1] - mat._m[2][1], _m[2][2] - mat._m[2][2], _m[2][3] - mat._m[2][3],
                    _m[3][0] - mat._m[3][0], _m[3][1] - mat._m[3][1], _m[3][2] - mat._m[3][2], _m[3][3] - mat._m[3][3]);
    }

    inline Mat4 Mat4::operator*(const Mat4& mat) const noexcept {
        Mat4 result;
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                for (size_t k = 0; k < 4; k++)
                    result._m[i][j] += _m[i][k] * mat._m[k][j];
        return result;
    }

    inline Mat4 Mat4::operator+(float s) const noexcept {
        Mat4 result;
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                result._m[i][j] = _m[i][j] + s;
        return result;
    }

    inline Mat4 Mat4::operator-(float s) const noexcept {
        Mat4 result;
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                result._m[i][j] = _m[i][j] - s;
        return result;
    }

    inline Mat4 Mat4::operator*(float s) const noexcept {
        Mat4 result;
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                result._m[i][j] = _m[i][j] * s;
        return result;
    }

    inline Vec4 Mat4::operator*(const Vec4& vec) const noexcept {
        return Vec4(_m[0][0] * vec.x + _m[0][1] * vec.y + _m[0][2] * vec.z + _m[0][3] * vec.w,
                    _m[1][0] * vec.x + _m[1][1] * vec.y + _m[1][2] * vec.z + _m[1][3] * vec.w,
                    _m[2][0] * vec.x + _m[2][1] * vec.y + _m[2][2] * vec.z + _m[2][3] * vec.w,
                    _m[3][0] * vec.x + _m[3][1] * vec.y + _m[3][2] * vec.z + _m[3][3] * vec.w);
    }

    inline Mat4& Mat4::operator+=(const Mat4& mat) noexcept {
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                _m[i][j] += mat._m[i][j];
        return *this;
    }

    inline Mat4& Mat4::operator-=(const Mat4& mat) noexcept {
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                _m[i][j] -= mat._m[i][j];
        return *this;
    }

    inline Mat4& Mat4::operator*=(const Mat4& mat) noexcept {
        *this = (*this) * mat;
        return *this;
    }

    inline Mat4& Mat4::operator+=(float s) noexcept {
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                _m[i][j] += s;
        return *this;
    }

    inline Mat4& Mat4::operator-=(float s) noexcept {
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                _m[i][j] -= s;
        return *this;
    }

    inline Mat4& Mat4::operator*=(float s) noexcept {
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                _m[i][j] *= s;
        return *this;
    }

    inline float Mat4::minor(size_t i, size_t j) const noexcept {
        if (i > 3 || j > 3) return 0.0f;
        return _m[3 - i][3 - j];
    }
    inline float Mat4::cofactor(size_t i, size_t j) const noexcept {
        float mij = minor(i, j);
        return ((i + j) % 2 == 0) ? mij : -mij;
    }

    inline float Mat4::det() const noexcept {
        float det00 = _m[1][1] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) -
            _m[1][2] * (_m[2][1] * _m[3][3] - _m[2][3] * _m[3][1]) +
            _m[1][3] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]);

        float det01 = _m[1][0] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) -
            _m[1][2] * (_m[2][0] * _m[3][3] - _m[2][3] * _m[3][0]) +
            _m[1][3] * (_m[2][0] * _m[3][2] - _m[2][2] * _m[3][0]);

        float det02 = _m[1][0] * (_m[2][1] * _m[3][3] - _m[2][3] * _m[3][1]) -
            _m[1][1] * (_m[2][0] * _m[3][3] - _m[2][3] * _m[3][0]) +
            _m[1][3] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]);

        float det03 = _m[1][0] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]) -
            _m[1][1] * (_m[2][0] * _m[3][2] - _m[2][2] * _m[3][0]) +
            _m[1][2] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]);

        return _m[0][0] * det00 - _m[0][1] * det01 + _m[0][2] * det02 - _m[0][3] * det03;
    }
    inline Mat4 Mat4::transposed() const noexcept {
        Mat4 result;
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                result._m[i][j] = _m[j][i];
        return result;
    }
    inline Mat4 Mat4::inversed() const noexcept {
        float determinant = det();
        if (fabsf(determinant) < Consts::EPS) return Mat4();

        float invDet = 1.0f / determinant;

        float cofactor00 = _m[1][1] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) -
            _m[1][2] * (_m[2][1] * _m[3][3] - _m[2][3] * _m[3][1]) +
            _m[1][3] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]);

        float cofactor01 = -(_m[1][0] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) -
            _m[1][2] * (_m[2][0] * _m[3][3] - _m[2][3] * _m[3][0]) +
            _m[1][3] * (_m[2][0] * _m[3][2] - _m[2][2] * _m[3][0]));

        float cofactor02 = _m[1][0] * (_m[2][1] * _m[3][3] - _m[2][3] * _m[3][1]) -
            _m[1][1] * (_m[2][0] * _m[3][3] - _m[2][3] * _m[3][0]) +
            _m[1][3] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]);

        float cofactor03 = -(_m[1][0] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]) -
            _m[1][1] * (_m[2][0] * _m[3][2] - _m[2][2] * _m[3][0]) +
            _m[1][2] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]));

        float cofactor10 = -(_m[0][1] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) -
            _m[0][2] * (_m[2][1] * _m[3][3] - _m[2][3] * _m[3][1]) +
            _m[0][3] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]));

        float cofactor11 = _m[0][0] * (_m[2][2] * _m[3][3] - _m[2][3] * _m[3][2]) -
            _m[0][2] * (_m[2][0] * _m[3][3] - _m[2][3] * _m[3][0]) +
            _m[0][3] * (_m[2][0] * _m[3][2] - _m[2][2] * _m[3][0]);

        float cofactor12 = -(_m[0][0] * (_m[2][1] * _m[3][3] - _m[2][3] * _m[3][1]) -
            _m[0][1] * (_m[2][0] * _m[3][3] - _m[2][3] * _m[3][0]) +
            _m[0][3] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]));

        float cofactor13 = _m[0][0] * (_m[2][1] * _m[3][2] - _m[2][2] * _m[3][1]) -
            _m[0][1] * (_m[2][0] * _m[3][2] - _m[2][2] * _m[3][0]) +
            _m[0][2] * (_m[2][0] * _m[3][1] - _m[2][1] * _m[3][0]);

        float cofactor20 = _m[0][1] * (_m[1][2] * _m[3][3] - _m[1][3] * _m[3][2]) -
            _m[0][2] * (_m[1][1] * _m[3][3] - _m[1][3] * _m[3][1]) +
            _m[0][3] * (_m[1][1] * _m[3][2] - _m[1][2] * _m[3][1]);

        float cofactor21 = -(_m[0][0] * (_m[1][2] * _m[3][3] - _m[1][3] * _m[3][2]) -
            _m[0][2] * (_m[1][0] * _m[3][3] - _m[1][3] * _m[3][0]) +
            _m[0][3] * (_m[1][0] * _m[3][2] - _m[1][2] * _m[3][0]));

        float cofactor22 = _m[0][0] * (_m[1][1] * _m[3][3] - _m[1][3] * _m[3][1]) -
            _m[0][1] * (_m[1][0] * _m[3][3] - _m[1][3] * _m[3][0]) +
            _m[0][3] * (_m[1][0] * _m[3][1] - _m[1][1] * _m[3][0]);

        float cofactor23 = -(_m[0][0] * (_m[1][1] * _m[3][2] - _m[1][2] * _m[3][1]) -
            _m[0][1] * (_m[1][0] * _m[3][2] - _m[1][2] * _m[3][0]) +
            _m[0][2] * (_m[1][0] * _m[3][1] - _m[1][1] * _m[3][0]));

        float cofactor30 = -(_m[0][1] * (_m[1][2] * _m[2][3] - _m[1][3] * _m[2][2]) -
            _m[0][2] * (_m[1][1] * _m[2][3] - _m[1][3] * _m[2][1]) +
            _m[0][3] * (_m[1][1] * _m[2][2] - _m[1][2] * _m[2][1]));

        float cofactor31 = _m[0][0] * (_m[1][2] * _m[2][3] - _m[1][3] * _m[2][2]) -
            _m[0][2] * (_m[1][0] * _m[2][3] - _m[1][3] * _m[2][0]) +
            _m[0][3] * (_m[1][0] * _m[2][2] - _m[1][2] * _m[2][0]);

        float cofactor32 = -(_m[0][0] * (_m[1][1] * _m[2][3] - _m[1][3] * _m[2][1]) -
            _m[0][1] * (_m[1][0] * _m[2][3] - _m[1][3] * _m[2][0]) +
            _m[0][3] * (_m[1][0] * _m[2][1] - _m[1][1] * _m[2][0]));

        float cofactor33 = _m[0][0] * (_m[1][1] * _m[2][2] - _m[1][2] * _m[2][1]) -
            _m[0][1] * (_m[1][0] * _m[2][2] - _m[1][2] * _m[2][0]) +
            _m[0][2] * (_m[1][0] * _m[2][1] - _m[1][1] * _m[2][0]);

        return Mat4(
            cofactor00 * invDet, cofactor10 * invDet, cofactor20 * invDet, cofactor30 * invDet,
            cofactor01 * invDet, cofactor11 * invDet, cofactor21 * invDet, cofactor31 * invDet,
            cofactor02 * invDet, cofactor12 * invDet, cofactor22 * invDet, cofactor32 * invDet,
            cofactor03 * invDet, cofactor13 * invDet, cofactor23 * invDet, cofactor33 * invDet
        );
    }
    inline Mat4 Mat4::abs() const noexcept {
        Mat4 result;
        for (size_t i = 0; i < 4; i++)
            for (size_t j = 0; j < 4; j++)
                result._m[i][j] = fabsf(_m[i][j]);
        return result;
    }

    inline Mat4 Mat4::identity() noexcept {
        return Mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 Mat4::scaling(float sx, float sy, float sz) noexcept {
        return Mat4(
            sx, 0.0f, 0.0f, 0.0f,
            0.0f, sy, 0.0f, 0.0f,
            0.0f, 0.0f, sz, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 Mat4::scaling(const Vec3& sv) noexcept {
        return Mat4(
            sv.x, 0.0f, 0.0f, 0.0f,
            0.0f, sv.y, 0.0f, 0.0f,
            0.0f, 0.0f, sv.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 Mat4::rotationX(float angle) noexcept {
        float cs = cosf(angle);
        float sn = sinf(angle);

        return Mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cs, -sn, 0.0f,
            0.0f, sn, cs, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 Mat4::rotationY(float angle) noexcept {
        float cs = cosf(angle);
        float sn = sinf(angle);

        return Mat4(
            cs, 0.0f, sn, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sn, 0.0f, cs, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 Mat4::rotationZ(float angle) noexcept {
        float cs = cosf(angle);
        float sn = sinf(angle);

        return Mat4(
            cs, -sn, 0.0f, 0.0f,
            sn, cs, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    inline Mat4 Mat4::translation(float tx, float ty, float tz) noexcept {
        return Mat4(
            1.0f, 0.0f, 0.0f, tx,
            0.0f, 1.0f, 0.0f, ty,
            0.0f, 0.0f, 1.0f, tz,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }
    inline Mat4 Mat4::translation(const Vec3& tv) noexcept {
        return Mat4(
            1.0f, 0.0f, 0.0f, tv.x,
            0.0f, 1.0f, 0.0f, tv.y,
            0.0f, 0.0f, 1.0f, tv.z,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }
    inline Mat4 Mat4::perspective(float fov, float aspect, float near, float far) noexcept {
        float h = 1.0f / tanf(fov / 2.0f);
        float zDist = far - near;
        return Mat4(
            h / aspect, 0, 0, 0,
            0, h, 0, 0,
            0, 0, far / zDist, -near * far / zDist,
            0, 0, 1, 0
        );
    }
    inline Mat4 Mat4::view(const Vec3& right, const Vec3& up, const Vec3& forward, const Vec3& pos) {
        return Mat4(
            right.x, right.y, right.z, -right.dot(pos),
            up.x, up.y, up.z, -up.dot(pos),
            forward.x, forward.y, forward.z, -forward.dot(pos),
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }
    inline Mat4 Mat4::view(const Vec3& forward, const Vec3& pos) {
        Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
        Vec3 right = up.cross(forward).normalized();
        up = forward.cross(right).normalized();
        return view(right, up, forward.normalized(), pos);
    }
    inline Mat4 Mat4::screenSpace(int width, int height, int offsetX, int offsetY) noexcept {
        float halfWidth = static_cast<float>(width) / 2.0f;
        float halfHeight = static_cast<float>(height) / 2.0f;

        return Mat4(
            halfWidth, 0, 0, halfWidth + offsetX,
            0, -halfHeight, 0, halfHeight + offsetY,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    }
}