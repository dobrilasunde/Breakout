#include "StartMenuLevel.h"

#include "Core/Engine.h"
#include "Core/Window.h"
#include "GameLevel.h"
#include "Objects/UI/UIButton.h"
#include "Objects/UI/UIObject.h"

#include "SFML/Graphics/Text.hpp"

void StartMenuLevel::OnCreated()
{
	assert(EngineProxy::IsValid());

	// create title
	const sf::Font& font = EngineProxy::Get().GetFont();
	sf::Text title = sf::Text{ "BREAKOUT", font, TITLE_SIZE };

	float titleX = static_cast<float>(Window::WIDTH / 2.0f);
	float titleZ = TITLE_OFFSET_Y;
	ObjectHandle<UIObject> textObject = AddObjectToLevel(EngineStatics::CreateObject<UIObject>(sf::Vector2f{ titleX, titleZ }));
	ObjectRef<UIObject> titleRef = textObject.lock();
	titleRef->SetText(title);


	float startButtonX = static_cast<float>(Window::WIDTH / 2.0f);;
	float startButtonY = titleZ + START_BUTTON_OFFSET_Y;
	sf::Text startButton = sf::Text{ "START", font, BUTTON_SIZE };
	startButton.setStyle(sf::Text::Bold);
	ObjectHandle<UIButton> startButtonObject = AddObjectToLevel(EngineStatics::CreateObject<UIButton>(sf::Vector2f{ startButtonX, startButtonY }));
	ObjectRef<UIButton> buttonRef = startButtonObject.lock();
	buttonRef->SetText(startButton);
	buttonRef->RegisterOnClickCallback(this, &StartMenuLevel::OnStartButtonClicked);
	buttonRef->SetBaseColor(sf::Color{ TEXT_COLOR_BASE });
	buttonRef->SetHoveredColor(sf::Color{ TEXT_COLOR_HOVER });
	buttonRef->SetPlaySoundOnHover(true);

	float quitButtonX = static_cast<float>(Window::WIDTH / 2.0f);;
	float quitButtonY = startButtonY + BUTTON_SPACING;
	sf::Text quitButton = sf::Text{ "QUIT", font, BUTTON_SIZE };
	quitButton.setStyle(sf::Text::Bold);
	ObjectHandle<UIButton> quitButtonObject = AddObjectToLevel(EngineStatics::CreateObject<UIButton>(sf::Vector2f{ quitButtonX, quitButtonY }));
	ObjectRef<UIButton> quitButtonRef = quitButtonObject.lock();
	quitButtonRef->SetText(quitButton);
	quitButtonRef->RegisterOnClickCallback(this, &StartMenuLevel::OnQuitButtonClicked);
	quitButtonRef->SetBaseColor(sf::Color{ TEXT_COLOR_BASE });
	quitButtonRef->SetHoveredColor(sf::Color{ TEXT_COLOR_HOVER });
	quitButtonRef->SetPlaySoundOnHover(true);

	sf::Text instructions = sf::Text{ "[LEFT CLICK] to fire ball, [MOUSE] to move the paddle, [ESC] to pause", font, INSTRUCTIONS_SIZE };

	float instructionsX = static_cast<float>(Window::WIDTH / 2.0f);
	float instructionsY = INSTRUCTIONS_OFFSET_Y;
	ObjectHandle<UIObject> instructionsObject = AddObjectToLevel(EngineStatics::CreateObject<UIObject>(sf::Vector2f{ instructionsX, instructionsY }));
	ObjectRef<UIObject> instructionsRef = instructionsObject.lock();
	instructionsRef->SetText(instructions);
}

void StartMenuLevel::OnStartButtonClicked()
{
	assert(EngineProxy::IsValid());

	EngineProxy::Get().StartLevel<GameLevel>();
}

void StartMenuLevel::OnQuitButtonClicked()
{
	assert(CurrentWindowProxy::IsValid());

	CurrentWindowProxy::Get().Close();

}
