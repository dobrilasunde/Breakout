#pragma once

#include "Core/GameObject.h"
#include "Util/Proxy.h"
#include "Core/Camera.h"
#include "UI/UIButton.h"
#include "Core/ObjectsCommon.h"

#include "SFML/Audio/Sound.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

enum class EGameState : uint8_t
{
	Default,
	Ingame,
	Paused,
	Won,
	Lost,
};

class GameManager : public GameObject
{
	using Super = GameObject;

public:
	static constexpr unsigned int TEXT_SIZE = 20;
	static constexpr unsigned int GAME_OVER_TEXT_SIZE = 80;
	static constexpr unsigned int SCORE_TEXT_SIZE = 120;
	static constexpr float GAME_BOUNDS_MARGIN = 25.f;
	static constexpr float GAME_OVER_TEXT_OFFSET_X = 40.0f; // from menu
	static constexpr float GAME_OVER_TEXT_OFFSET_Y = 30.0f; // from menu
	static constexpr int BUTTON_SIZE = 30;
	static constexpr float RESTART_BUTTON_OFFSET_Y = 30.0f; // from game over text
	static constexpr float BUTTON_SPACING_Y = 60.0f; // spacing between two buttons
	static constexpr sf::Uint32 TEXT_COLOR_BASE = 0xffffffff;
	static constexpr sf::Uint32 TEXT_COLOR_HOVER = 0xf9c74fff;
	static constexpr sf::Uint32 OVERLAY_COLOR = 0x277da1c8;

public:
	GameManager(int8_t updateOrder = 0):
		Super(updateOrder)
	{
		
	}

	void BeginPlay() override;
	void Update(float deltaSeconds) override;
	void Draw(Window& window) override;
	void EndPlay() override;

	void OnBrickDestroyed();
	void OnBallLost();

	void SetNbBricks(int32_t nbBricks) { _nbBricks = nbBricks; }

	EGameState GetGameState() const { return _gameState; }

private:
	int32_t _lives = 3;
	int32_t _score = 0;
	int32_t _scoreIncrement = 10;
	int32_t _nbBricks = 0;

	Camera _camera;

	sf::Text _textLives;
	sf::Text _textScore;
	sf::Sound _brickBreakSound;

	sf::RectangleShape _boundsShape;

	sf::RectangleShape _menuOverlay;
	sf::Text _textBigScore;
	sf::Text _textGameState;

	std::vector<ObjectHandle<UIButton>> _menuButtons;

	EGameState _gameState = EGameState::Default;

private:
	void GameOver(bool victory);

	void OnGameWon();
	void OnGameLost();

	void OnGamePaused();
	void OnGameResumed();

	void CreateMenuButtons();
	void DestroyMenuButtons();

	void OnRestartButtonClicked();
	void OnQuitButtonClicked();

	void Input_OnPausePressed();
};

struct GameManagerProxy : public Proxy<GameManager>
{
	GameManagerProxy();
};