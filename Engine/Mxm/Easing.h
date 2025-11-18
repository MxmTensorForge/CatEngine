#ifndef EASING_H
#define EASING_H

#include <cmath>
#include "Consts.h"

namespace Mxm
{
	namespace Func {
		float easeInQuad(float x) noexcept;
		float easeOutQuad(float x) noexcept;

		float easeInCubic(float x) noexcept;
		float easeOutCubic(float x) noexcept;

		float easeInQuart(float x) noexcept;
		float easeOutQuart(float x) noexcept;

		float easeInSine(float x) noexcept;
		float easeOutSine(float x) noexcept;

		float easeExp(float x, float a) noexcept;

		float easeInCirc(float x) noexcept;
		float easeOutCirc(float x) noexcept;

		float cosBounceIn(float x, float bounces, float decay) noexcept;
		float cosBounceOut(float x, float bounces, float decay) noexcept;

		float elasticCosBounceIn(float x, float bounces, float decay) noexcept;
		float elasticCosBounceIn(float x, float bounces, float decay) noexcept;
	}
}

#include "src/Easing.inl"

#endif // !FUNC_H
