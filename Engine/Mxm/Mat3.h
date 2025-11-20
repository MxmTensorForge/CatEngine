#ifndef MAT3_H
#define MAT3_H

#include <cmath>
#include "Vec3.h"

namespace Mxm
{
    class Mat3 final
    {
    private:
        float _m[3][3];
    public:
        Mat3() noexcept;
        explicit Mat3(float a00, float a01, float a02,
            float a10, float a11, float a12,
            float a20, float a21, float a22) noexcept;
        explicit Mat3(const Vec3& vec1, const Vec3& vec2, const Vec3& vec3) noexcept;

        Vec3 operator[](size_t index) const noexcept;
        Vec3 col(size_t index) const noexcept;

        Mat3 operator+(const Mat3& mat) const noexcept;
        Mat3 operator-(const Mat3& mat) const noexcept;
        Mat3 operator*(const Mat3& mat) const noexcept;

        Mat3 operator+(float s) const noexcept;
        Mat3 operator-(float s) const noexcept;
        Mat3 operator*(float s) const noexcept;

        Vec3 operator*(const Vec3& vec) const noexcept;

        Mat3& operator+=(const Mat3& mat) noexcept;
        Mat3& operator-=(const Mat3& mat) noexcept;
        Mat3& operator*=(const Mat3& mat) noexcept;

        Mat3& operator+=(float s) noexcept;
        Mat3& operator-=(float s) noexcept;
        Mat3& operator*=(float s) noexcept;

        float minor(size_t i, size_t j) const noexcept;
        float cofactor(size_t i, size_t j) const noexcept;

        float det() const noexcept;
        Mat3 transposed() const noexcept;
        Mat3 inversed() const noexcept;
        Mat3 abs() const noexcept;

        static Mat3 identity() noexcept;
        static Mat3 scaling(float sx, float sy, float sz) noexcept;
        static Mat3 scaling(const Vec3& sv) noexcept;

        static Mat3 rotationX(float angle) noexcept;
        static Mat3 rotationY(float angle) noexcept;
        static Mat3 rotationZ(float angle) noexcept;
    };
}

#include "src/Mat3.inl"

#endif // !MAT3_H