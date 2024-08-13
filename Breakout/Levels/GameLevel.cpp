#include "Core/Engine.h"
#include "Core/EngineModules/ObjectEngineModule.h"
#include "Core/Window.h"
#include "GameLevel.h"
#include "Objects/Ball.h"
#include "Objects/Brick.h"
#include "Objects/Paddle.h"

#include "SFML/Audio/Music.hpp"
#include "SFML/Window/Mouse.hpp"

void GameLevel::OnStarted()
{
	Super::OnStarted();

	ObjectHandle<GameManager> gameManager = AddObjectToLevel(EngineStatics::CreateObject<GameManager>(GameObject::MAX_UPDATE_ORDER - 1));

	float paddleX = static_cast<float>(Window::WIDTH / 2.0f);
	float paddleY = static_cast<float>(Window::HEIGHT - Paddle::HEIGHT / 2.0f - MARGIN_OFFSET_Y);
	ObjectHandle<Paddle> paddle = AddObjectToLevel(EngineStatics::CreateObject<Paddle>(sf::Vector2f{ paddleX, paddleY }));

	int bricksColumn = static_cast<int>((Window::WIDTH - 2 * MARGIN_OFFSET_X) / (Brick::WIDTH + BRICK_SPACING));
	int bricksRow = static_cast<int>((Window::HEIGHT / 3.0f) / (Brick::HEIGHT + BRICK_SPACING));

	_nbBricks = bricksColumn * bricksRow;
	ObjectRef<GameManager> gameManagerRef = gameManager.lock();
	gameManagerRef->SetNbBricks(_nbBricks);

	static const std::vector<sf::Color> BRICK_COLORS = {
		sf::Color{0xf94144ff},
		sf::Color{0xf3722cff},
		sf::Color{0xf8961eff},
		sf::Color{0xf9844aff},
		sf::Color{0xf9c74fff},
		sf::Color{0x90be6dff},
		sf::Color{0x43aa8bff},
		sf::Color{0x4d908eff},
		sf::Color{0x577590ff},
		sf::Color{0x277da1ff}
	};

	for (int i = 0; i < bricksColumn; ++i)
	{
		for (int j = 0; j < bricksRow; ++j)
		{
			int colorIndex = static_cast<int>(static_cast<float>(j) / (bricksRow - 1) * (BRICK_COLORS.size() - 1));
			const sf::Color color = BRICK_COLORS[colorIndex];

			float brickX = static_cast<float>(MARGIN_OFFSET_X + Brick::WIDTH / 2.0f + i * Brick::WIDTH + i * BRICK_SPACING);
			float brickY = static_cast<float>(MARGIN_OFFSET_Y + Brick::HEIGHT / 2.0f + j * Brick::HEIGHT + j * BRICK_SPACING);
			ObjectHandle<Brick> brick = AddObjectToLevel(EngineStatics::CreateObject<Brick>(sf::Vector2f{ brickX, brickY }, color));
		}
	}

	CurrentWindowProxy::Get().SetMouseCursorVisible(false);
	sf::Mouse::setPosition(sf::Vector2i{ Window::WIDTH / 2, Window::HEIGHT / 2 }, CurrentWindowProxy::Get());

	assert(EngineProxy::IsValid());

	sf::Music& gameMusic = EngineProxy::Get().GetMusic();
	gameMusic.setLoop(true);
	gameMusic.play();
}
