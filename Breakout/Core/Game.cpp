#include "Game.h"

#include "Core/Window.h"
#include "Core/Engine.h"
#include "Levels/GameLevel.h"
#include "Levels/StartMenuLevel.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <string>

void Game::Initialize()
{
}

void Game::RunLoop()
{
	Window gameWindow;
	CurrentWindowProxy::Open(gameWindow);

	Engine gameEngine;
	gameEngine.Initialize();

	EngineProxy::Open(gameEngine);
	
	// fps debug
	{
		const sf::Font& font = gameEngine.GetFont();
		_fpsText = sf::Text{ "fps: ", font, 15 };
	}

	static const float UPDATES_PER_SECOND = 60.f;
	static const sf::Time TARGET_DELTA_SECONDS = sf::seconds(1.f / UPDATES_PER_SECOND);

	sf::Clock clock;
	sf::Time accumulator;
	
	// Init level
	gameEngine.StartLevel<StartMenuLevel>();

	while (gameWindow.IsOpen())
	{
		gameWindow.PollEvents();
		gameWindow.Clear();

		accumulator += clock.restart();

		while (accumulator > TARGET_DELTA_SECONDS)
		{
			gameEngine.Update(TARGET_DELTA_SECONDS.asSeconds());
			accumulator -= TARGET_DELTA_SECONDS;
		}

		gameEngine.Draw(gameWindow);

		CalculateFPS();
		gameWindow.Draw(_fpsText);

		gameWindow.Display();
	}

	CurrentWindowProxy::Close(gameWindow);
	EngineProxy::Close(gameEngine);
}

void Game::CalculateFPS()
{
	static sf::Clock timer;
	static uint32_t numFrames = 0;

	numFrames++;

	static const float MEASURE_PERIOD_SECONDS = 0.5f;
	if (timer.getElapsedTime().asSeconds() >= MEASURE_PERIOD_SECONDS)
	{
		float fps = static_cast<float>(numFrames) / timer.getElapsedTime().asSeconds();
		_fpsText.setString("fps: " + std::to_string(fps));

		numFrames = 0;
		timer.restart();
	}
}

