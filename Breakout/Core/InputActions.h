#pragma once

#include <cstdint>

enum class EInputAction : uint8_t
{
	LaunchBall,
	ButtonClick,
	Pause,

	EnableDebug,

	// Keep last
	Count
};

enum class EInputType
{
	Pressed,
	Released
};
