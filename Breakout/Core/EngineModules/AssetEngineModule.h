#pragma once

#include <string>

#include "Core/EngineModule.h"
#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

struct SFXSet
{
	sf::SoundBuffer _paddleHit;
	sf::SoundBuffer _wallHit;
	sf::SoundBuffer _brickHit;
	sf::SoundBuffer _brickBreak;
	sf::SoundBuffer _menuButtonHover;
};

class AssetEngineModule : public EngineModule
{
	using Super = EngineModule;

public:
	void LoadAllAssets();

	const sf::Font& GetFont() const { return _font; }
	sf::Music& GetMusic() { return _gameMusic; }

	SFXSet& GetSFXSet() { return _sfxSet; }

private:
	static constexpr const char* FONT_PATH = "./Content/fonts/cour.ttf";
	static constexpr const char* MUSIC_PATH = "./Content/sounds/theme.flac";
	static constexpr const char* SFX_PADDLE_HIT_PATH = "./Content/sounds/paddle_hit.wav";
	static constexpr const char* SFX_WALL_HIT_PATH = "./Content/sounds/brick_hit.wav";
	static constexpr const char* SFX_BRICK_HIT_PATH = "./Content/sounds/brick_hit.wav";
	static constexpr const char* SFX_BRICK_BREAK_PATH = "./Content/sounds/brick_break.wav";
	static constexpr const char* SFX_MENU_BUTTTON_HOVER_PATH = "./Content/sounds/menu_hover.wav";

	sf::Texture _tileSet;
	sf::Font _font;
	sf::Music _gameMusic;

	SFXSet _sfxSet;
};