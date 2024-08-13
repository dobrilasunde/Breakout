#include "Core/GameObject.h"
#include "PhysicsEngineModule.h"
#include "Core/Window.h"

#include "SFML/Graphics/RectangleShape.hpp"

#include <algorithm>
#include <cassert>

PhysicsEngineModule::PhysicsEngineModule()
{
	_inputHandler.RegisterInputCallback(this, EInputAction::EnableDebug, EInputType::Pressed, &PhysicsEngineModule::Input_EnableDebug);
}

void PhysicsEngineModule::OnUpdate(float deltaSeconds)
{
	Super::OnUpdate(deltaSeconds);

	_inputHandler.ProcessInputs();

	// Collision detection
	for (CollisionLayerType i = 0; i < static_cast<CollisionLayerType>(ECollisionLayer::Count); ++i)
	{
		const ECollisionLayer collisionLayerA = static_cast<ECollisionLayer>(i);

		for (CollisionLayerType j = i; j < static_cast<CollisionLayerType>(ECollisionLayer::Count); ++j)
		{
			const ECollisionLayer collisionLayerB = static_cast<ECollisionLayer>(j);

			if (!_collisionMatrix.AreLayersColliding(collisionLayerA, collisionLayerB))
			{
				continue;
			}

			// Check collision between layers - check each object combination of the two layers
			const std::vector<ObjectHandle<GameObject>>& lhs = _collisionObjectsPerLayer[i];
			const std::vector<ObjectHandle<GameObject>>& rhs = _collisionObjectsPerLayer[j];
			for (int32_t lhsIdx = 0; lhsIdx < lhs.size(); ++lhsIdx)
			{
				for (int32_t rhsIdx = 0; rhsIdx < rhs.size(); ++rhsIdx)
				{
					ObjectRef<GameObject> lhsObj = lhs[lhsIdx].lock();
					ObjectRef<GameObject> rhsObj = rhs[rhsIdx].lock();

					assert(lhsObj);
					assert(rhsObj);

					// Check if objects colliding
					const AABB lhsAABB = lhsObj->GetCollisionBounds() + lhsObj->GetPosition();
					const AABB rhsAABB = rhsObj->GetCollisionBounds() + rhsObj->GetPosition();
					if (lhsAABB.Overlaps(rhsAABB))
					{
						lhsObj->OnCollision(*rhsObj.get());
						rhsObj->OnCollision(*lhsObj.get());
					}
				}
			}
		}
	}
}

void PhysicsEngineModule::OnDraw(Window& window)
{
	if (_drawDebug)
	{
		DrawDebug(window);
	}
}

void PhysicsEngineModule::EnableCollisionForObject(ObjectHandle<GameObject> object)
{
	ObjectRef<GameObject> objectRef = object.lock();
	assert(objectRef);

	ECollisionLayer objectLayer = objectRef->GetCollisionLayer();
	std::vector<ObjectHandle<GameObject>>& collisionObjectsPerLayer = _collisionObjectsPerLayer[static_cast<CollisionLayerType>(objectLayer)];

	// Add object to map, if not exists.
	if (std::find_if(collisionObjectsPerLayer.begin(), collisionObjectsPerLayer.end(), [&objectRef](const ObjectHandle<GameObject>& o) {
		ObjectRef<GameObject> oRef = o.lock();
		return oRef == objectRef;
		}) == collisionObjectsPerLayer.end())
	{
		collisionObjectsPerLayer.emplace_back(object);
	}
}

void PhysicsEngineModule::DisableCollisionForObject(ObjectHandle<GameObject> object)
{
	ObjectRef<GameObject> objectRef = object.lock();
	assert(objectRef);

	ECollisionLayer objectLayer = objectRef->GetCollisionLayer();
	std::vector<ObjectHandle<GameObject>>& collisionObjectsPerLayer = _collisionObjectsPerLayer[static_cast<CollisionLayerType>(objectLayer)];

	// Remove object from map.
	collisionObjectsPerLayer.erase(std::remove_if(collisionObjectsPerLayer.begin(), collisionObjectsPerLayer.end(), [&objectRef](const ObjectHandle<GameObject>& o) {
		ObjectRef<GameObject> oRef = o.lock();
		return oRef == objectRef;
		}), collisionObjectsPerLayer.end());
}

void PhysicsEngineModule::Input_EnableDebug()
{
	_drawDebug = !_drawDebug;
}

void PhysicsEngineModule::DrawDebug(Window& window)
{
	static const std::array<sf::Color, static_cast<CollisionLayerType>(ECollisionLayer::Count)> colorPerLayer = {
		sf::Color::White,
		sf::Color::Green,
		sf::Color::Yellow,
		sf::Color::Cyan
	};

	static constexpr float boundsThickness = 2.f;

	sf::RectangleShape collisionShape;
	collisionShape.setFillColor(sf::Color::Transparent);
	collisionShape.setOutlineThickness(boundsThickness);


	for (CollisionLayerType i = 0; i < static_cast<CollisionLayerType>(ECollisionLayer::Count); ++i)
	{
		const std::vector<ObjectHandle<GameObject>>& objects = _collisionObjectsPerLayer[i];
		for (const ObjectHandle<GameObject>& objectHandle : objects)
		{
			const ObjectRef<GameObject> object = objectHandle.lock();
			const AABB aabb = object->GetCollisionBounds() + object->GetPosition();
			sf::Vector2f size = aabb.GetSize();
			size.x -= boundsThickness * 2.f;
			size.y -= boundsThickness * 2.f;

			collisionShape.setOutlineColor(colorPerLayer[i]);
			collisionShape.setPosition(aabb.GetCenter());
			collisionShape.setSize(size);
			collisionShape.setOrigin(size * 0.5f);
			window.Draw(collisionShape);
		}
	}
}
