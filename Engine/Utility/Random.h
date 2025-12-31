#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>

namespace Random {
	void init(uint32_t seed);

	float range(float min, float max);
	float value();
	int range(int min, int max);

	bool flag(float change);
}

#endif // !RANDOM_H