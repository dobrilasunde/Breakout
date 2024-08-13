#pragma once

#include "Core/GameObject.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Audio/Sound.hpp"

class Brick : public GameObject
{
	using Super = GameObject;

public:
	static constexpr float HEIGHT = 40.0f;
	static constexpr float WIDTH = 40.0f;

public:
	Brick(const sf::Vector2f& position, const sf::Color& color) :
		_shape{ sf::Vector2f{ WIDTH, HEIGHT } },
		_color{ color }
	{
		SetPosition(position);
		_collisionLayer = ECollisionLayer::Brick;
	}

	void BeginPlay() override;
	void Draw(Window& window) override;

	void OnCollision(GameObject& otherObject) override;

private:
	sf::Color _color;

	sf::RectangleShape _shape;

	int _health = 2;
};