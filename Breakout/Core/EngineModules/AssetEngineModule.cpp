#include "AssetEngineModule.h"

#include <cassert>

void AssetEngineModule::LoadAllAssets()
{
	if (!_font.loadFromFile(FONT_PATH)) { assert(false); }
	if (!_gameMusic.openFromFile(MUSIC_PATH)) { assert(false); }

	if (!_sfxSet._paddleHit.loadFromFile(SFX_PADDLE_HIT_PATH)) { assert(false); }
	if (!_sfxSet._wallHit.loadFromFile(SFX_WALL_HIT_PATH)) { assert(false); }
	if (!_sfxSet._brickHit.loadFromFile(SFX_BRICK_HIT_PATH)) { assert(false); }
	if (!_sfxSet._brickBreak.loadFromFile(SFX_BRICK_BREAK_PATH)) { assert(false); }
	if (!_sfxSet._menuButtonHover.loadFromFile(SFX_MENU_BUTTTON_HOVER_PATH)) { assert(false); }
}
