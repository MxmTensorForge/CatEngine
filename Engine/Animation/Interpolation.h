#ifndef INTERPOLATION_H
#define INTERPOLATION_H

namespace Mxm { struct Vec2; }

namespace Interpolation
{
    float Linear(float t) noexcept;
    float Cos(float t) noexcept;
    float AbsCosBounce(float t) noexcept;
    float CosBounce(float t) noexcept;
    float EasyOut(float t) noexcept;
    float Bezier(float t, const Mxm::Vec2& p0, const Mxm::Vec2& p1) noexcept;
    float Custom(float t) noexcept;
}

#endif // !INTERPOLATION_H
