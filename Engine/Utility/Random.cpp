#include "Random.h"

#include <random>

namespace Random {
	static std::mt19937 rng;
	static bool initialized = false;

	void init(uint32_t seed) {
		rng.seed(seed);
		initialized = true;
	}

	float range(float min, float max) {
		if (!initialized) init(5489u);
		std::uniform_real_distribution<float> dist(min, max);
		return dist(rng);
	}
	float value() {
		return range(0.0f, 1.0f);
	}
	int range(int min, int max) {
		if (!initialized) init(5489u);
		std::uniform_int_distribution<int> dist(min, max);
		return dist(rng);
	}

	bool flag(float change) {
		return range(0.0f, 1.0f) < change;
	}
}