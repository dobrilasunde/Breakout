#include "Paddle.h"

#include "Ball.h"
#include "Core/Engine.h"
#include "Core/GameObject.h"
#include "Core/Level.h"
#include "Core/ObjectsCommon.h"
#include "Core/Window.h"
#include "Util/Math.h"
#include "GameManager.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Mouse.hpp"

Paddle::Paddle(const sf::Vector2f& position, int8_t updateOrder/* = 0*/) :
	Super{ updateOrder },
	_shape{ sf::Vector2f{ WIDTH, HEIGHT } }
{
	SetPosition(position);
	_collisionLayer = ECollisionLayer::Paddle;
}

void Paddle::BeginPlay()
{
	EnableCollision();
	SetCollisionBounds(sf::Vector2f{ -WIDTH/2.0f - BOUNDS_TOLERANCE, -HEIGHT/2.0f - BOUNDS_TOLERANCE }, sf::Vector2f{ WIDTH/2.0f + BOUNDS_TOLERANCE, HEIGHT/2.0f + BOUNDS_TOLERANCE });

	_shape.setOrigin(sf::Vector2f{ WIDTH / 2.0f, HEIGHT / 2.0f });
	_shape.setFillColor(sf::Color(BASE_COLOR_VALUE));

	SpawnBall();

	// Setup inputs
	GetInputHandler().RegisterInputCallback(this, EInputAction::LaunchBall, EInputType::Pressed, &Paddle::Input_OnLaunchBall);
}

void Paddle::Update(float deltaSeconds)
{
	Super::Update(deltaSeconds);

	if (GameManagerProxy::Get().GetGameState() != EGameState::Ingame)
	{
		return;
	}

	assert(CurrentWindowProxy::IsValid());

	if (!CurrentWindowProxy::Get().IsInFocus())
	{
		return;
	}

	if (_ball.expired())
	{
		SpawnBall();
	}

	_lastMousePosition = sf::Vector2i{ Window::WIDTH / 2, Window::HEIGHT / 2 };

	const sf::Vector2i mousePos = sf::Mouse::getPosition(CurrentWindowProxy::Get());
	const sf::Vector2i mouseDelta = mousePos - _lastMousePosition;
	_lastMousePosition = mousePos;

	const sf::Vector2f velocity{
		static_cast<float>(mouseDelta.x),
		0.f
	};

	static constexpr float SPEED = 50.f;
	sf::Vector2f newPosition = GetPosition() + velocity * SPEED * deltaSeconds;
	newPosition.x = Clamp(newPosition.x, 100.f, Window::WIDTH - 100.f);
	SetPosition(newPosition);

	if (!_ballReleased)
	{
		UpdateBallPosition(newPosition);
	}

	// Update color "animation"
	static const  sf::Color BASE_COLOR { BASE_COLOR_VALUE };
	static const sf::Color HIGHLIGHT_COLOR{ HIGHLIGHT_COLOR_VALUE };

	const sf::Color color{
		Lerp(_colorOffsetNormalized, BASE_COLOR.r, HIGHLIGHT_COLOR.r),
		Lerp(_colorOffsetNormalized, BASE_COLOR.g, HIGHLIGHT_COLOR.g),
		Lerp(_colorOffsetNormalized, BASE_COLOR.b, HIGHLIGHT_COLOR.b),
		Lerp(_colorOffsetNormalized, BASE_COLOR.a, HIGHLIGHT_COLOR.a)
	};

	_shape.setFillColor(color);

	_colorOffsetNormalized = Clamp(_colorOffsetNormalized * (1.f - deltaSeconds * HIGHLIGHT_SPEED), 0.f, 1.f);

	sf::Mouse::setPosition(sf::Vector2i{ Window::WIDTH / 2, Window::HEIGHT / 2 }, CurrentWindowProxy::Get());
}

void Paddle::Draw(Window& window)
{
	Super::Draw(window);

	_shape.setPosition(GetPosition());
	window.Draw(_shape);
}

void Paddle::OnCollision(GameObject& otherObject)
{
	if (otherObject.GetCollisionLayer() == ECollisionLayer::Ball)
	{
		_colorOffsetNormalized = 1.f;
	}
}

void Paddle::SpawnBall()
{
	_ballReleased = false;

	float ballX = GetPosition().x;
	float ballY = GetPosition().y - HEIGHT / 2.0f - Ball::RADIUS * 2.0f + 1.f;
	_ball = GetParentLevel()->AddObjectToLevel(EngineStatics::CreateObject<Ball>(sf::Vector2{ ballX, ballY }));
}

void Paddle::Input_OnLaunchBall()
{
	if (_ballReleased)
	{
		return;
	}

	ObjectRef<Ball> ballRef = _ball.lock();
	ballRef->SetVelocity(sf::Vector2f{ 0.0f, Ball::MAX_SPEED * -1.0f });

	_ballReleased = true;
}

void Paddle::UpdateBallPosition(const sf::Vector2f& paddlePosition)
{
	ObjectRef<Ball> ballRef = _ball.lock();
	float ballX = paddlePosition.x;
	float ballY = paddlePosition.y - Ball::RADIUS * 2.0f + 1.f;
	ballRef->SetPosition(sf::Vector2f{ ballX, ballY });
}

