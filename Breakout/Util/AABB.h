#pragma once

#include "SFML/System/Vector2.hpp"

inline bool SegmentOverlap(float minA, float maxA, float minB, float maxB)
{
	return minB < maxA && minA < maxB;
}

// Axis-aligned bounding box.
struct AABB
{
	sf::Vector2f Min = sf::Vector2f{ 0.f, 0.f };
	sf::Vector2f Max = sf::Vector2f{ 0.f, 0.f };

	bool IsZero() const
	{
		const sf::Vector2f size = Max - Min;
		return std::abs(size.x) < 1e-6f && std::abs(size.y) < 1e-6f;
	}

	bool Overlaps(const AABB& other) const
	{
		if (IsZero() || other.IsZero())
		{
			return false;
		}

		return SegmentOverlap(Min.x, Max.x, other.Min.x, other.Max.x) &&
			SegmentOverlap(Min.y, Max.y, other.Min.y, other.Max.y);
	}

	AABB CalculateOverlap(const AABB& other) const
	{
		if (!Overlaps(other))
		{
			return {};
		}

		const sf::Vector2f min {
			std::max(Min.x, other.Min.x),
			std::max(Min.y, other.Min.y)
		};
		const sf::Vector2f max{
			std::min(Max.x, other.Max.x),
			std::min(Max.y, other.Max.y)
		};

		return AABB{ min, max };
	}

	sf::Vector2f GetCenter() const { return (Min + Max) * 0.5f; }
	sf::Vector2f GetSize() const { return Max - Min; }

	AABB operator+(sf::Vector2f vector) const
	{
		return AABB{ Min + vector, Max + vector };
	}
};