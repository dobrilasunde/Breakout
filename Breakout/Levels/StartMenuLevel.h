#pragma once
#include "Core/Level.h"
#include "SFML/Config.hpp"

class StartMenuLevel : public Level
{
public:
	virtual void OnCreated() override;

private:
	static constexpr int TITLE_SIZE = 200;
	static constexpr float TITLE_OFFSET_Y = 200.0f;

	static constexpr int BUTTON_SIZE = 30;
	static constexpr float START_BUTTON_OFFSET_Y = 250.0f;
	static constexpr float BUTTON_SPACING = 100.0f;

	static constexpr int INSTRUCTIONS_SIZE = 20;
	static constexpr float INSTRUCTIONS_OFFSET_Y = 700.0f;

	static constexpr sf::Uint32 TEXT_COLOR_BASE = 0xffffffff;
	static constexpr sf::Uint32 TEXT_COLOR_HOVER = 0xf9c74fff;

private:
	void OnStartButtonClicked();
	void OnQuitButtonClicked();
};