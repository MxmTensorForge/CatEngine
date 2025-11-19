#ifndef ENGINECONSTS_H
#define ENGINECONSTS_H

namespace EngineConsts
{
	constexpr int STANDART_WIDTH = 800;
	constexpr int STANDART_HEIGHT = 600;

	constexpr float SCREEN_SCALE = 0.5f;

	constexpr int SCALED_WIDTH = STANDART_WIDTH * SCREEN_SCALE;
	constexpr int SCALED_HEIGHT = STANDART_HEIGHT * SCREEN_SCALE;
}

#endif // !ENGINECONSTS_H
