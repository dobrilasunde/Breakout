#include "Ball.h"

#include "Core/Engine.h"
#include "Core/Window.h"
#include "Util/Math.h"
#include "Core/Level.h"
#include "OBjects/GameManager.h"

#include "SFML/Graphics/CircleShape.hpp"

#include <cmath>


void Ball::BeginPlay()
{
	Super::BeginPlay();

	assert(EngineProxy::IsValid());

	EnableCollision();
	SetCollisionBounds(sf::Vector2f{-_radius, -_radius}, sf::Vector2f{ _radius, _radius });

	_paddleHitSound.setBuffer(EngineProxy::Get().GetSFXSet()._paddleHit);
	_wallHitSound.setBuffer(EngineProxy::Get().GetSFXSet()._wallHit);
	_brickHitSound.setBuffer(EngineProxy::Get().GetSFXSet()._brickHit);

	_shape.setRadius(_radius);
	_shape.setOrigin(sf::Vector2f{ _radius, _radius });
	_shape.setFillColor(sf::Color::White);

	if (Level* parentLevel = GetParentLevel())
	{
		ParticleSystem::ParticleParams params;
		params.InitialNumParticles = 0;
		params.ParticlesSpawnPeriodSeconds = 0.05f;

		params.InitialDirectionMin = sf::Vector2f {0.f, 0.f};
		params.InitialDirectionMax = sf::Vector2f {0.f, 0.f};

		params.InitialSpeedMin = 0.f;
		params.InitialSpeedMax = 0.f;
		params.Gravity = 0.f;

		params.AlphaFalloffPerSecond = 3.f;

		params.RandomColor = false;
		params.Color = sf::Color(0xdadadaff);

		params.AutoDestroy = false;

		params.Shape = ParticleSystem::EParticleShape::Sphere;
		params.Size = RADIUS * 0.3f;
		params.SizeFalloffPerSecond = 10.f;

		_particles = parentLevel->AddObjectToLevel(EngineStatics::CreateObject<ParticleSystem>(GetPosition(), GetUpdateOrder() - 1, params));
		if (ObjectRef<ParticleSystem> particles = _particles.lock())
		{
			particles->SetActive(false);
		}
	}
}

void Ball::Update(float deltaSeconds)
{
	Super::Update(deltaSeconds);

	if (GameManagerProxy::Get().GetGameState() != EGameState::Ingame)
	{
		return;
	}

	const sf::Vector2f newPosition = GetPosition() + _velocity * deltaSeconds;
	SetPosition(newPosition);

	// Floor
	if (newPosition.y > Window::HEIGHT + _radius)
	{
		assert(GameManagerProxy::IsValid());
		GameManagerProxy::Get().OnBallLost();

		Destroy();
		_particles.lock()->Destroy();
		return;
	}

	// Out of bounds
	if (newPosition.x < _radius + GameManager::GAME_BOUNDS_MARGIN || newPosition.x > Window::WIDTH - _radius - GameManager::GAME_BOUNDS_MARGIN)
	{
		_velocity.x *= -1.0f;
		_wallHitSound.play();
	}
	if (newPosition.y < _radius + GameManager::GAME_BOUNDS_MARGIN)
	{
		_velocity.y *= -1.0f;
		_wallHitSound.play();
	}

	_velocity.x = Clamp(_velocity.x, -MAX_SPEED, MAX_SPEED);
	_velocity.y = Clamp(_velocity.y, -MAX_SPEED, MAX_SPEED);

	sf::Vector2f position = GetPosition();
	position.x = Clamp(position.x, _radius, Window::WIDTH - _radius);
	position.y = Clamp(position.y, _radius, position.y);
	SetPosition(position);

	if (ObjectRef<ParticleSystem> particles = _particles.lock())
	{
		particles->SetPosition(GetPosition());
		particles->SetActive(GetVectorSizeSquared(_velocity) > SMALL_NUMBER);
	}
}

void Ball::Draw(Window& window)
{
	Super::Draw(window);

	_shape.setPosition(GetPosition());
	window.Draw(_shape);
}

void Ball::OnCollision(GameObject& otherObject)
{
	Super::OnCollision(otherObject);

	if (GetVectorSizeSquared(_velocity) < 1e-6f)
	{
		return;
	}

	const AABB thisAABB = GetCollisionBounds() + GetPosition();
	const AABB otherABB = otherObject.GetCollisionBounds() + otherObject.GetPosition();
	const AABB overlap = thisAABB.CalculateOverlap(otherABB);

	if (otherObject.GetCollisionLayer() == ECollisionLayer::Paddle)
	{
		OnPaddleCollision(otherObject);
	}
	else if (otherObject.GetCollisionLayer() == ECollisionLayer::Brick)
	{
		OnBrickCollision(otherObject);
	}
	else
	{
		assert(false);
	}

	// "Uncollide"
	const sf::Vector2f overlapSize = overlap.GetSize();
	const sf::Vector2f collisionVector = otherObject.GetPosition() - GetPosition();

	const float penetrationX = overlapSize.x * (std::signbit(collisionVector.x) ? -1.f : 1.f);
	const float penetrationY = overlapSize.y * (std::signbit(collisionVector.y) ? -1.f : 1.f);

	const float translationX = overlapSize.x <= overlapSize.y + SMALL_NUMBER ? -penetrationX : 0.f;
	const float translationY = overlapSize.x >= overlapSize.y - SMALL_NUMBER ? -penetrationY : 0.f;

	SetPosition(GetPosition() + sf::Vector2f{ translationX, translationY });
}

void Ball::OnPaddleCollision(GameObject& paddle)
{
	const AABB thisAABB = GetCollisionBounds() + GetPosition();
	const AABB paddleAABB = paddle.GetCollisionBounds() + paddle.GetPosition();
	const AABB overlap = thisAABB.CalculateOverlap(paddleAABB);
	const sf::Vector2f collisionPoint = overlap.GetCenter();	// global

	// Collisions on the side - normal bounce
	const sf::Vector2f overlapSize = overlap.GetSize();
	if (overlapSize.x <= overlapSize.y + SMALL_NUMBER)
	{
		_velocity.x *= -1.f;
		return;
	}

	assert(collisionPoint.x > paddleAABB.Min.x);
	assert(collisionPoint.x < paddleAABB.Max.x);

	float third = (paddleAABB.Max.x - paddleAABB.Min.x) / 3.0f;
	float half = (paddleAABB.Max.x - paddleAABB.Min.x) / 2.0f;
	float firstThird = paddleAABB.Min.x + third;
	float secondThird = paddleAABB.Min.x + 2 * third;

	// Heuristically chosen constants an interesting bounce.
	if (collisionPoint.x >= paddleAABB.Min.x && collisionPoint.x < firstThird)
	{
		_velocity = GetReflectionVector(_velocity, sf::Vector2f{ -0.196f, -0.981f });

	}
	else if (collisionPoint.x >= firstThird && collisionPoint.x <= secondThird)
	{
		_velocity = GetReflectionVector(_velocity, sf::Vector2f{ 0.0f, -1.0f });

	}
	else if (collisionPoint.x > secondThird && collisionPoint.x <= paddleAABB.Max.x)
	{
		_velocity = GetReflectionVector(_velocity, sf::Vector2f{ 0.196f, -0.981f });
	}
	else
	{
		assert(false);
	}

	_paddleHitSound.play();
}

void Ball::OnBrickCollision(GameObject& brick)
{
	const AABB thisAABB = GetCollisionBounds() + GetPosition();
	const AABB brickAABB = brick.GetCollisionBounds() + brick.GetPosition();
	const AABB overlap = thisAABB.CalculateOverlap(brickAABB);
	const sf::Vector2f collisionPoint = overlap.GetCenter();	// global

	const sf::Vector2f overlapSize = overlap.GetSize();

	if (std::abs(overlapSize.x - overlapSize.y) < SMALL_NUMBER)
	{
		_velocity.x *= -1.f;
		_velocity.y *= -1.f;
	}
	else if (overlapSize.x > overlapSize.y)
	{
		_velocity.y *= -1.0f;
	}
	else if (overlapSize.x < overlapSize.y)
	{
		_velocity.x *= -1.0f;
	}

	_brickHitSound.play();
}
