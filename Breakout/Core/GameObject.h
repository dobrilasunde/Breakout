#pragma once

#include "Core/CollisionLayers.h"
#include "Util/AABB.h"
#include "Core/InputHandler.h"

class Level;
class Window;

class GameObject
{
public:
	static constexpr int8_t MIN_UPDATE_ORDER = std::numeric_limits<int8_t>::min();
	static constexpr int8_t MAX_UPDATE_ORDER = std::numeric_limits<int8_t>::max();

public:
	GameObject(int8_t updateOrder = 0) :
		_updateOrder{ updateOrder }
	{

	}

	virtual ~GameObject() = default;	

	virtual void BeginPlay() {};

	virtual void Update(float deltaSeconds);
	virtual void Draw(Window& window) {};

	virtual void EndPlay() {};

	void Destroy();

	ECollisionLayer GetCollisionLayer() const { return _collisionLayer; }
	bool IsCollisionEnabled() const { return _isCollisionEnabled; }
	void EnableCollision();
	void DisableCollision();

	const AABB& GetCollisionBounds() const { return _collisionBounds; }
	void SetCollisionBounds(sf::Vector2f min, sf::Vector2f max)
	{
		_collisionBounds.Min = min;
		_collisionBounds.Max = max;
	}
	void SetCollisionBounds(const AABB& aabb)
	{
		_collisionBounds = aabb;
	}

	sf::Vector2f GetPosition() const { return _position; }
	void SetPosition(sf::Vector2f value) { _position = value; }

	virtual void OnCollision(GameObject& otherObject) {}

	Level* GetParentLevel() const { return _parentLevel; }
	void SetParentLevel(Level* parentLevel)
	{
		_parentLevel = parentLevel;
	}

	int8_t GetUpdateOrder() const { return _updateOrder; }

protected:
	ECollisionLayer _collisionLayer = ECollisionLayer::Default;

protected:
	InputHandler& GetInputHandler() { return _inputHandler; }
	const InputHandler& GetInputHandler() const { return _inputHandler; }

private:
	InputHandler _inputHandler;

	sf::Vector2f _position = sf::Vector2f { 0.f, 0.f };

	bool _isCollisionEnabled = false;
	AABB _collisionBounds;

	Level* _parentLevel = nullptr;

	// Lower numbers get updated earlier.
	int8_t _updateOrder = 0;
};