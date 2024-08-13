#include "Objects/ParticleSystem.h"
#include "Util/Math.h"
#include "Core/Window.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"

ParticleSystem::ParticleSystem(sf::Vector2f position, int8_t updateOrder/* = 0*/, ParticleParams params /*= {}*/):
	Super{ updateOrder },
	_params{ params }
{
	SetPosition(position);
}

void ParticleSystem::BeginPlay()
{
	// Spawn particles
	for (int i = 0; i < _params.InitialNumParticles; ++i)
	{
		SpawnParticle();
	}
}

void ParticleSystem::Update(float deltaSeconds)
{
	Super::Update(deltaSeconds);

	// TODO - Params

	// Spawn particles
	if (_isActive && _params.ParticlesSpawnPeriodSeconds > 1e-6f)
	{
		_particleSpawnTimerSeconds += deltaSeconds;
		if (_particleSpawnTimerSeconds >= _params.ParticlesSpawnPeriodSeconds)
		{
			_particleSpawnTimerSeconds = 0.f;

			SpawnParticle();
		}
	}

	// Update particles
	for (int i = static_cast<int>(_particles.size() - 1); i >= 0; --i)
	{
		Particle& particle = _particles[i];

		particle.Velocity += sf::Vector2f{ 0.f, _params.Gravity } * deltaSeconds;
		particle.Position += particle.Velocity * deltaSeconds;
		particle.LifetimeSeconds += deltaSeconds;

		if (particle.LifetimeSeconds >= _params.Lifetime)
		{
			std::iter_swap(_particles.begin() + i, _particles.end() - 1);
			_particles.pop_back();
		}
	}

	if (_params.AutoDestroy && _particles.empty())
	{
		Destroy();
	}
}

void ParticleSystem::Draw(Window& window)
{
	Super::Draw(window);

	const ParticleParams& params = _params;
	auto DrawShape = [&window, &params](sf::Vector2f position, sf::Color color, float size) {
		switch (params.Shape)
		{
			case EParticleShape::Box:
			{
				sf::RectangleShape shape;
				shape.setSize(sf::Vector2f{ size, size });
				shape.setOrigin(sf::Vector2f{ size * 0.5f, size * 0.5f });
				shape.setPosition(position);
				shape.setFillColor(color);
				window.Draw(shape);

				break;
			}
			case EParticleShape::Sphere:
			{
				sf::CircleShape shape;
				shape.setRadius(size * 2.f);
				shape.setOrigin(sf::Vector2f{ size * 2.f, size * 2.f });
				shape.setPosition(position);
				shape.setFillColor(color);
				window.Draw(shape);

				break;
			}
		}
	};

	for (const Particle& particle : _particles)
	{
		sf::Color color = particle.Color;
		color.a = static_cast<sf::Uint8>(Clamp(1.f - particle.LifetimeSeconds * _params.AlphaFalloffPerSecond, 0.f, 1.f) * 255.f);

		const float size = std::max(0.f, _params.Size - particle.LifetimeSeconds * _params.SizeFalloffPerSecond);

		DrawShape(particle.Position, color, size);
	}
}

void ParticleSystem::SpawnParticle()
{
	// Spawn particle
	const sf::Vector2f direction{
		RandomRange(_params.InitialDirectionMin.x, _params.InitialDirectionMax.x),
		RandomRange(_params.InitialDirectionMin.y, _params.InitialDirectionMax.y)
	};

	const sf::Color color = _params.RandomColor ?
		sf::Color{ RandomRange<sf::Uint8>(0u, 255), RandomRange<sf::Uint8>(0u, 255), RandomRange<sf::Uint8>(0u, 255) } :
		_params.Color;

	_particles.emplace_back(
		GetPosition(),
		direction * RandomRange(_params.InitialSpeedMin, _params.InitialSpeedMax),
		color
	);
}
