#include "Interpolation.h"
#include "../Mxm/Consts.h"
#include "../Mxm/Vec2.h"

#include <cmath>

float Interpolation::Linear(float t) noexcept {
    return t;
}
float Interpolation::Cos(float t) noexcept {
    return (1.0f - cosf(Mxm::Consts::PI * t)) * 0.5f;
}
float Interpolation::AbsCosBounce(float t) noexcept {
    const float bounces = 3.0f;
    const float decay = 1.0f;

    float value = fabsf(cosf(t * bounces * Mxm::Consts::PI)) * expf(-decay * t);
    return 1.0f - value;
}
float Interpolation::CosBounce(float t) noexcept {
    const float bounces = 3.0f;
    const float decay = 1.0f;

    float value = cosf(t * bounces * Mxm::Consts::PI * 2.0f) * expf(-decay * t);
    return 1.0f - value;
}
float Interpolation::EasyOut(float t) noexcept {
    return 1.0f - expf(-t * 5.0f);
}

float Interpolation::Bezier(float t, const Mxm::Vec2& p0, const Mxm::Vec2& p1) noexcept {
    // ћы ищем все Bx(t_progress) такие, которые равны t, затем просто берем y компоненту этой точки и получаем наше нелинейное врем€
    // ¬ поиске t_progress помогает численный метод Ќьютона (Xn+1 = Xn - f(x) / f'(x)), т.е. Bx(t_progress) = t, приводим к стандартному виду: Bx(t_progress) - t = 0
    // ѕодставл€ем: t_progress_new = t_progress_old - (Bx(t_progress_old) - t) / Bx'(t_progress_old)
    Mxm::Vec2 start = Mxm::Vec2(0.0f, 0.0f);
    Mxm::Vec2 end = Mxm::Vec2(1.0f, 1.0f);

    auto sx = [=](float st) -> float {
        return (1.0f - st) * (1.0f - st) * (1.0f - st) * start.x + 3.0f * st * (1.0f - st) * (1.0f - st) * p0.x + 3.0f * st * st * (1.0f - st) * p1.x + st * st * st * end.x;
    };
    
    auto sy = [=](float st) -> float {
        return (1.0f - st) * (1.0f - st) * (1.0f - st) * start.y + 3.0f * st * (1.0f - st) * (1.0f - st) * p0.y + 3.0f * st * st * (1.0f - st) * p1.y + st * st * st * end.y;
    };
    
    auto sxd = [=](float st) -> float {
        return (sx(st + Mxm::Consts::EPS) - sx(st - Mxm::Consts::EPS)) / (2.0f * Mxm::Consts::EPS);
    };

    float ty = 0.5f;
    for (int i = 0; i < 8; i++) {
        float f = sx(ty) - t;
        if (fabsf(f) < Mxm::Consts::EPS) break;

        float df = sxd(ty);
        if (fabsf(df) < Mxm::Consts::EPS) break;

        ty = ty - f / df; //df равна sdx, т.к. константа t сокращаетс€ при дифференцировании
    }
    return sy(ty);
}

float Interpolation::Custom(float t) noexcept {
    return t * t * t;
}