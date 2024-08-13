#pragma once

#include "Core/GameObject.h"
#include "SFML/Graphics/Color.hpp"

class ParticleSystem : public GameObject
{
	using Super = GameObject;

public:
	enum class EParticleShape : uint8_t
	{
		Box,
		Sphere
	};

	struct ParticleParams
	{
		int InitialNumParticles = 50;
		float ParticlesSpawnPeriodSeconds = 0.f;	// 0 means no periodic spawn

		float InitialSpeedMin = 400.f;
		float InitialSpeedMax = 600.f;
		float Gravity = 1000.f;

		sf::Vector2f InitialDirectionMin = sf::Vector2f{ -1.f, -1.f };
		sf::Vector2f InitialDirectionMax = sf::Vector2f{ 1.f, 1.f };

		float Lifetime = 2.f;

		sf::Color Color = sf::Color::White;
		bool RandomColor = true;
		float AlphaFalloffPerSecond = 0.f;

		bool AutoDestroy = true;

		EParticleShape Shape = EParticleShape::Box;

		float Size = 10.f;
		float SizeFalloffPerSecond = 0.f;
	};

public:
	ParticleSystem(sf::Vector2f position, int8_t updateOrder = 0, ParticleParams params = {});

	void BeginPlay() override;
	void Update(float deltaSeconds) override;
	void Draw(Window& window) override;

	void SetActive(bool value) { _isActive = value; }

private:
	struct Particle
	{
		sf::Vector2f Position = sf::Vector2f{ 0.f, 0.f };
		sf::Vector2f Velocity = sf::Vector2f{ 0.f, 0.f };
		sf::Color Color = sf::Color::White;
		float LifetimeSeconds = 0.f;

		Particle(sf::Vector2f position, sf::Vector2f velocity, sf::Color color) :
			Position{ position },
			Velocity{ velocity },
			Color{ color }
		{

		}
	};

private:
	ParticleParams _params;

	std::vector<Particle> _particles;
	float _particleSpawnTimerSeconds = 0.f;

	bool _isActive = true;

private:
	void SpawnParticle();
};