#pragma once

#include "Core/GameObject.h"
#include "Objects/ParticleSystem.h"

#include "Core/ObjectsCommon.h"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"

class Ball : public GameObject
{
	using Super = GameObject;

public:
	static constexpr float RADIUS = 15.0f;
	static constexpr float MAX_SPEED = 800.0f;

public:
	Ball(const sf::Vector2f& position, int8_t updateOrder = 0):
		Super{ updateOrder }
	{
		SetPosition(position);
		_collisionLayer = ECollisionLayer::Ball;
	}

	void BeginPlay() override;
	void Update(float deltaSeconds) override;
	void Draw(Window& window) override;

	void OnCollision(GameObject& otherObject) override;

	void SetVelocity(const sf::Vector2f& velocity)
	{
		_velocity = velocity;
	}

	const sf::Vector2f& GetVelocity() const { return _velocity; }

private:
	sf::CircleShape _shape;
	ObjectHandle<ParticleSystem> _particles;

	sf::Vector2f _velocity = sf::Vector2f{ 0.0f, 0.0f };

	float _radius = RADIUS;

	sf::Sound _paddleHitSound;
	sf::Sound _wallHitSound;
	sf::Sound _brickHitSound;

private:
	void OnPaddleCollision(GameObject& paddle);
	void OnBrickCollision(GameObject& brick);
};