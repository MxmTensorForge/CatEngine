namespace Mxm
{
	namespace Ease {
		inline float easeInQuad(float x) noexcept {
			return x * x;
		}
		inline float easeOutQuad(float x) noexcept {
			float t = 1.0f - x;
			return 1.0f - t * t;
		}

		inline float easeInCubic(float x) noexcept {
			return x * x * x;
		}
		inline float easeOutCubic(float x) noexcept {
			float t = 1.0f - x;
			return 1.0f - t * t * t;
		}

		inline float easeInQuart(float x) noexcept {
			return x * x * x * x;
		}
		inline float easeOutQuart(float x) noexcept {
			float t = 1.0f - x;
			return 1.0f - t * t * t * t;
		}

		inline float easeInSine(float x) noexcept {
			return 1.0f - cosf(x * Consts::HALF_PI);
		}
		inline float easeOutSine(float x) noexcept {
			return sinf(x * Consts::HALF_PI);
		}

		inline float easeExp(float x, float a) noexcept {
			return (1.0f - expf(-x * a)) / (1.0f - expf(-a));
		}

		inline float easeInCirc(float x) noexcept {
			return 1.0f - sqrtf(1.0f - powf(x, 2.0f));
		}
		inline float easeOutCirc(float x) noexcept {
			return sqrtf(1.0f - powf(x - 1.0f, 2.0f));
		}

		inline float cosBounceIn(float x, float bounces, float decay) noexcept {
			float cos_val = cosf(x * Consts::PI * bounces);
			return 1.0f - fabsf(cos_val) * powf(1.0f - x, decay);
		}
		inline float cosBounceOut(float x, float bounces, float decay) noexcept {
			float t = 1.0f - x;
			float cos_val = cosf(t * Consts::PI * bounces);
			return 1.0f - fabsf(cos_val) * powf(t, decay);
		}

		inline float elasticCosBounceIn(float x, float bounces, float decay) noexcept {
			float cos_val = cosf(x * Consts::PI * bounces * 2.0f);
			return 1.0f - cos_val * powf(1.0f - x, decay);
		}
		inline float elasticCosBounceOut(float x, float bounces, float decay) noexcept {
			float t = 1.0f - x;
			float cos_val = cosf(t * Consts::PI * bounces * 2.0f);
			return 1.0f - cos_val * powf(t, decay);
		}
	}
}