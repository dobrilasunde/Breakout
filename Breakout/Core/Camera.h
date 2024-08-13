#pragma once
#include "SFML/System/Vector2.hpp"

class Camera
{
public:
	void Update(float deltaSeconds);
	void DoCameraShake();

	void SetCenter(const sf::Vector2f value) { _center = value; }

private:
	static constexpr float SHAKE_AMOUNT_X = 5.f;
	static constexpr float SHAKE_AMOUNT_Y = 10.f;
	static constexpr float SHAKE_RECOVERY_SPEED = 10.f;

	sf::Vector2f _center = sf::Vector2f{ 0.f, 0.f };
	sf::Vector2f _offset = sf::Vector2f{ 0.f, 0.f };
};