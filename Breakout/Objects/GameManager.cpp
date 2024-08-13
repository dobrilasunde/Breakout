#include "GameManager.h"

#include "Core/Engine.h"
#include "Core/GameObject.h"
#include "Core/Level.h"
#include "Core/Window.h"
#include "Levels/GameLevel.h"
#include "UI/UIButton.h"
#include "Levels/StartMenuLevel.h"

#include <cassert>
#include <string>

void GameManager::BeginPlay()
{
	Super::BeginPlay();

	assert(EngineProxy::IsValid());

	_textLives.setFont(EngineProxy::Get().GetFont());
	_textLives.setCharacterSize(TEXT_SIZE);
	_textLives.setPosition(sf::Vector2f{ 60.f, Window::HEIGHT - TEXT_SIZE - 15.f});

	_textScore.setFont(EngineProxy::Get().GetFont());
	_textScore.setCharacterSize(TEXT_SIZE);
	_textScore.setPosition(sf::Vector2f{ 220.f, Window::HEIGHT - TEXT_SIZE - 15.f });

	_brickBreakSound.setBuffer(EngineProxy::Get().GetSFXSet()._brickBreak);

	_boundsShape.setSize(sf::Vector2f{Window::WIDTH - GAME_BOUNDS_MARGIN * 2.f, Window::HEIGHT + GAME_BOUNDS_MARGIN});
	_boundsShape.setOrigin(sf::Vector2f{ 0.f, 0.f });
	_boundsShape.setPosition(sf::Vector2f{ GAME_BOUNDS_MARGIN, GAME_BOUNDS_MARGIN });
	_boundsShape.setOutlineThickness(2.f);
	_boundsShape.setOutlineColor(sf::Color::White);
	_boundsShape.setFillColor(sf::Color::Transparent);

	float menuPositionX = Window::WIDTH / 2.0f;
	float menuPositionY = Window::HEIGHT / 2.0f;
	_menuOverlay.setSize(sf::Vector2f{ Window::WIDTH, Window::HEIGHT});
	_menuOverlay.setOrigin(_menuOverlay.getSize() * 0.5f);
	_menuOverlay.setPosition(sf::Vector2f{ menuPositionX, menuPositionY });

	_textBigScore.setFont(EngineProxy::Get().GetFont());
	_textBigScore.setCharacterSize(SCORE_TEXT_SIZE);
	_textBigScore.setStyle(sf::Text::Bold);
	_textBigScore.setPosition(sf::Vector2f{ menuPositionX, menuPositionY - 250.f});

	_textGameState.setFont(EngineProxy::Get().GetFont());
	_textGameState.setCharacterSize(GAME_OVER_TEXT_SIZE);
	_textGameState.setPosition(sf::Vector2f{ menuPositionX, menuPositionY });
	
	_menuOverlay.setFillColor(sf::Color{ OVERLAY_COLOR });

	assert(CurrentWindowProxy::IsValid());

	_camera.SetCenter(CurrentWindowProxy::Get().GetView().getCenter());

	GetInputHandler().RegisterInputCallback(this, EInputAction::Pause, EInputType::Pressed, &GameManager::Input_OnPausePressed);

	_gameState = EGameState::Ingame;

	GameManagerProxy::Open(*this);
}

void GameManager::Update(float deltaSeconds)
{
	Super::Update(deltaSeconds);

	_textLives.setString("Lives: " + std::to_string(_lives));
	_textScore.setString("Score: " + std::to_string(_score));

	_camera.Update(deltaSeconds);
}

void GameManager::Draw(Window& window)
{
	Super::Draw(window);

	window.Draw(_textLives);
	window.Draw(_textScore);
	
	window.Draw(_boundsShape);

	if (_gameState == EGameState::Won || 
		_gameState == EGameState::Lost ||
		_gameState == EGameState::Paused
		)
	{
		window.Draw(_menuOverlay);

		_textBigScore.setString("SCORE: " + std::to_string(_score));
		_textBigScore.setOrigin(_textBigScore.getLocalBounds().getSize() * 0.5f);
		window.Draw(_textBigScore);

		window.Draw(_textGameState);

	}
}

void GameManager::EndPlay()
{
	Super::EndPlay();

	GameManagerProxy::Close(*this);
}

void GameManager::OnBrickDestroyed()
{
	_brickBreakSound.play();
	_camera.DoCameraShake();

	_score += _scoreIncrement;
	_nbBricks--;

	if (_nbBricks <= 0)
	{
		GameOver(true);
	}
}

void GameManager::OnBallLost()
{
	_lives--;

	if (_lives <= 0)
	{
		GameOver(false);
	}
}

void GameManager::GameOver(bool victory)
{
	CreateMenuButtons();
	CurrentWindowProxy::Get().SetMouseCursorVisible(true);

	if (victory)
	{
		_gameState = EGameState::Won;
		OnGameWon();
	}
	else
	{
		_gameState = EGameState::Lost;
		OnGameLost();
	}
}


void GameManager::OnGameWon()
{
	_textGameState.setString("FLAWLESS VICTORY");
	_textGameState.setOrigin(_textGameState.getLocalBounds().getSize() * 0.5f);
}

void GameManager::OnGameLost()
{
	_textGameState.setString("GAME OVER");
	_textGameState.setOrigin(_textGameState.getLocalBounds().getSize() * 0.5f);
}

void GameManager::OnGamePaused()
{
	_textGameState.setString("PAUSE");
	_textGameState.setOrigin(_textGameState.getLocalBounds().getSize() * 0.5f);

	CreateMenuButtons();
	CurrentWindowProxy::Get().SetMouseCursorVisible(true);
}

void GameManager::OnGameResumed()
{
	DestroyMenuButtons();
	CurrentWindowProxy::Get().SetMouseCursorVisible(false);
}

void GameManager::CreateMenuButtons()
{
	assert(EngineProxy::IsValid());

	// Restart button
	float restartButtonX = _textGameState.getPosition().x;
	float restartButtonY = _textGameState.getPosition().y + GAME_OVER_TEXT_SIZE + RESTART_BUTTON_OFFSET_Y;
	sf::Text restartButton = sf::Text{ "MAIN MENU", EngineProxy::Get().GetFont(), BUTTON_SIZE };
	ObjectHandle<UIButton> restartButtonObject = GetParentLevel()->AddObjectToLevel(EngineStatics::CreateObject<UIButton>(sf::Vector2f{ restartButtonX, restartButtonY }, GameObject::MAX_UPDATE_ORDER));
	_menuButtons.emplace_back(restartButtonObject);

	ObjectRef<UIButton> restartButtonRef = restartButtonObject.lock();
	restartButtonRef->SetText(restartButton);
	restartButtonRef->RegisterOnClickCallback(this, &GameManager::OnRestartButtonClicked);
	restartButtonRef->SetPlaySoundOnHover(true);
	restartButtonRef->SetBaseColor(sf::Color{ TEXT_COLOR_BASE });
	restartButtonRef->SetHoveredColor(sf::Color{ TEXT_COLOR_HOVER });

	// Quit button
	float quitButtonX = _textGameState.getPosition().x;
	float quitButtonY = restartButtonY + BUTTON_SIZE + BUTTON_SPACING_Y;
	sf::Text quitButton = sf::Text{ "QUIT", EngineProxy::Get().GetFont(), BUTTON_SIZE };
	ObjectHandle<UIButton> quitButtonObject = GetParentLevel()->AddObjectToLevel(EngineStatics::CreateObject<UIButton>(sf::Vector2f{ quitButtonX, quitButtonY }, GameObject::MAX_UPDATE_ORDER));
	_menuButtons.emplace_back(quitButtonObject);

	ObjectRef<UIButton> quitButtonRef = quitButtonObject.lock();
	quitButtonRef->SetText(quitButton);
	quitButtonRef->RegisterOnClickCallback(this, &GameManager::OnQuitButtonClicked);
	quitButtonRef->SetPlaySoundOnHover(true);
	quitButtonRef->SetBaseColor(sf::Color{ TEXT_COLOR_BASE });
	quitButtonRef->SetHoveredColor(sf::Color{ TEXT_COLOR_HOVER });
}

void GameManager::DestroyMenuButtons()
{
	for (const ObjectHandle<UIButton>& button : _menuButtons)
	{
		button.lock()->Destroy();
	}

	_menuButtons.clear();
}

void GameManager::OnRestartButtonClicked()
{
	assert(EngineProxy::IsValid());

	EngineProxy::Get().StartLevel<StartMenuLevel>();
}

void GameManager::OnQuitButtonClicked()
{
	assert(CurrentWindowProxy::IsValid());

	CurrentWindowProxy::Get().Close();
}

void GameManager::Input_OnPausePressed()
{
	assert(EngineProxy::IsValid());

	if (_gameState == EGameState::Ingame)
	{
		_gameState = EGameState::Paused;
		OnGamePaused();
	}
	else if (_gameState == EGameState::Paused)
	{
		_gameState = EGameState::Ingame;
		OnGameResumed();
	}
}

GameManagerProxy::GameManagerProxy() = default;

