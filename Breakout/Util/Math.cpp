#include "Math.h"

#include <math.h>

sf::Vector2f GetSafeNormal(sf::Vector2f vector, float tolerance /*= SMALL_NUMBER*/)
{
	const float squareSum = vector.x * vector.x + vector.y * vector.y;

	if (squareSum == 1.f)
	{
		return vector;
	}
	else if (squareSum < tolerance)
	{
		return {};
	}

	const float scale = 1.0f / sqrtf(squareSum);
	return sf::Vector2f{ vector.x * scale, vector.y * scale };

	return vector;
}

sf::Vector2f GetReflectionVector(sf::Vector2f direction, sf::Vector2f surfaceNormal)
{
	sf::Vector2f safeNormal = GetSafeNormal(surfaceNormal);

	return direction - 2 * DotProduct(direction, safeNormal) * safeNormal;
}
