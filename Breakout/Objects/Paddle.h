#pragma once

#include "Ball.h"
#include "Core/GameObject.h"
#include "Core/ObjectsCommon.h"

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"

class Paddle : public GameObject
{
	using Super = GameObject;

public:
	static constexpr float HEIGHT = 20.0f;
	static constexpr float WIDTH = 146.0f;
	static constexpr sf::Uint32 BASE_COLOR_VALUE = 0xf9c74fff;
	static constexpr sf::Uint32 HIGHLIGHT_COLOR_VALUE = 0xffffffff;
	static constexpr float HIGHLIGHT_SPEED = 15.f;
	static constexpr float BOUNDS_TOLERANCE = 5.f;

public:
	Paddle(const sf::Vector2f& position, int8_t updateOrder = 0);

	void BeginPlay() override;
	void Update(float deltaSeconds) override;
	void Draw(Window& window) override;

	void OnCollision(GameObject& otherObject) override;

private:
	sf::RectangleShape _shape;
	
	sf::Vector2i _lastMousePosition;

	bool _ballReleased = false;
	ObjectHandle<Ball> _ball;

	float _colorOffsetNormalized = 0.f;

private:
	void SpawnBall();
	void Input_OnLaunchBall();
	void UpdateBallPosition(const sf::Vector2f& paddlePosition);
};