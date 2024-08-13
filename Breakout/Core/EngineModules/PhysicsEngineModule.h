#pragma once

#include "Core/EngineModule.h"
#include "Core/CollisionLayers.h"
#include "Core/ObjectsCommon.h"
#include "Core/InputHandler.h"

#include <vector>
#include <array>

class PhysicsEngineModule : public EngineModule
{
	using Super = EngineModule;

public:
	PhysicsEngineModule();

	void OnUpdate(float deltaSeconds) override;
	void OnDraw(Window& window) override;

	void EnableCollisionForObject(ObjectHandle<GameObject> object);
	void DisableCollisionForObject(ObjectHandle<GameObject> object);

private:
	using CollisionLayerType = std::underlying_type_t<ECollisionLayer>;

	static constexpr CollisionMatrix _collisionMatrix = []() -> CollisionMatrix {
		CollisionMatrix collisionMatrix;

		collisionMatrix.EnableCollision(ECollisionLayer::Ball, ECollisionLayer::Brick);
		collisionMatrix.EnableCollision(ECollisionLayer::Ball, ECollisionLayer::Paddle);

		return collisionMatrix;
	}();

	std::array<std::vector<ObjectHandle<GameObject>>, static_cast<CollisionLayerType>(ECollisionLayer::Count)> _collisionObjectsPerLayer;

	InputHandler _inputHandler;

	bool _drawDebug = false;

private:
	void Input_EnableDebug();

	void DrawDebug(Window& window);
};