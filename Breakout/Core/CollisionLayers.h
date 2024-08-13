#pragma once

#include <cstdint>
#include <array>
#include <cassert>

enum class ECollisionLayer : uint8_t
{
	Default,
	Ball,
	Brick,
	Paddle,

	// keep this last
	Count
};

class CollisionMatrix
{

public:
	constexpr CollisionMatrix():
		_collisionMatrix{false}
	{
	}

	constexpr void EnableCollision(ECollisionLayer layer1, ECollisionLayer layer2)
	{
		_collisionMatrix[static_cast<CollisionLayerType>(layer1)][static_cast<CollisionLayerType>(layer2)] = true;
		_collisionMatrix[static_cast<CollisionLayerType>(layer2)][static_cast<CollisionLayerType>(layer1)] = true;
	}

	constexpr void DisableCollision(ECollisionLayer layer1, ECollisionLayer layer2)
	{
		_collisionMatrix[static_cast<CollisionLayerType>(layer1)][static_cast<CollisionLayerType>(layer2)] = false;
		_collisionMatrix[static_cast<CollisionLayerType>(layer2)][static_cast<CollisionLayerType>(layer1)] = false;
	}

	constexpr bool AreLayersColliding(ECollisionLayer layer1, ECollisionLayer layer2) const
	{
		const bool value1 = _collisionMatrix[static_cast<CollisionLayerType>(layer1)][static_cast<CollisionLayerType>(layer2)];
		const bool value2 = _collisionMatrix[static_cast<CollisionLayerType>(layer2)][static_cast<CollisionLayerType>(layer1)];
		assert(value1 == value2);

		return value1;
	}

private:
	using CollisionLayerType = std::underlying_type_t<ECollisionLayer>;
	static constexpr CollisionLayerType NUM_LAYERS = static_cast<CollisionLayerType>(ECollisionLayer::Count);

	std::array<std::array<bool, NUM_LAYERS>, NUM_LAYERS> _collisionMatrix;
};