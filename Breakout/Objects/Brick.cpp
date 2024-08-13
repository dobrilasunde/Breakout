#include "Brick.h"

#include "Core/Engine.h"
#include "Core/Level.h"
#include "Core/Window.h"
#include "GameManager.h"
#include "ParticleSystem.h"
#include "Util/Math.h"

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"

void Brick::BeginPlay()
{
	Super::BeginPlay();

	EnableCollision();
	SetCollisionBounds(sf::Vector2f{ -WIDTH / 2.0f, -HEIGHT / 2.0f }, sf::Vector2f{ WIDTH / 2.0f, HEIGHT / 2.0f });

	_shape.setOrigin(sf::Vector2f{ WIDTH / 2.0f, HEIGHT / 2.0f });
	_shape.setFillColor(_color);
}

void Brick::Draw(Window& window)
{
	Super::Draw(window);

	_shape.setPosition(GetPosition());

	window.Draw(_shape);
}

void Brick::OnCollision(GameObject& otherObject)
{
	Super::OnCollision(otherObject);

	int numParticles = 5;
	_health--;
	sf::Color particleColor = sf::Color::White;

	if (_health == 1)
	{
		_color.r = Clamp(_color.r + 80, 0, 255);
		_color.g = Clamp(_color.g + 80, 0, 255);
		_color.b = Clamp(_color.b + 80, 0, 255);
		_shape.setFillColor(_color);
	}

	if (_health <= 0)
	{
		numParticles = 20;
		particleColor = _color;

		assert(GameManagerProxy::IsValid());
		GameManagerProxy::Get().OnBrickDestroyed();

		Destroy();
	}

	if (Level* parentLevel = GetParentLevel())
	{
		ParticleSystem::ParticleParams params;
		params.InitialNumParticles = numParticles;
		params.AlphaFalloffPerSecond = 1.f;
		params.RandomColor = false;
		params.Color = particleColor;

		ObjectHandle<ParticleSystem> particles = parentLevel->AddObjectToLevel(EngineStatics::CreateObject<ParticleSystem>(GetPosition(), GameObject::MAX_UPDATE_ORDER, params));
	}

}

