#pragma once

#include "SFML/System/Vector2.hpp"
#include <algorithm>
#include <cmath>

#define SMALL_NUMBER (1.e-8f)

template <typename T>
T Clamp(const T& value, const T& min, const T& max)
{
	return std::min(max, std::max(min, value));
}

template<typename T, typename Enable = void>
T Lerp(float t, const T& min, const T& max)
{
	return min + static_cast<T>(t * (max - min));
}

inline float DotProduct(sf::Vector2f left, sf::Vector2f right)
{
	return left.x * right.x + left.y * right.y;
}

// Returns normalized vector. For zero vectors, return zero.
sf::Vector2f GetSafeNormal(sf::Vector2f vector, float tolerance = SMALL_NUMBER);

// Return reflection vector for given surface.
sf::Vector2f GetReflectionVector(sf::Vector2f direction, sf::Vector2f surfaceNormal);

inline float GetVectorSizeSquared(sf::Vector2f vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

inline float GetVectorSize(sf::Vector2f vector)
{
	return std::sqrt(GetVectorSizeSquared(vector));
}

template <typename T>
T RandomRange(T min, T max)
{
	return static_cast<T>(RandomRange<float>(static_cast<float>(min), static_cast<float>(max)));
}

template <>
inline float RandomRange<float>(float min, float max)
{
	return static_cast<float>(std::rand()) / RAND_MAX * (max - min) + min;
}