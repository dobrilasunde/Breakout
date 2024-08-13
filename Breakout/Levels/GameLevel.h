#pragma once
#include "Core/Level.h"
#include "Objects/GameManager.h"

class GameLevel : public Level
{
	using Super = Level;

public:
	virtual void OnStarted() override;
	int32_t GetNbBricks() const { return _nbBricks; }

private:
	static constexpr float MARGIN_OFFSET_X = 200.0f;
	static constexpr float MARGIN_OFFSET_Y = 100.0f;
	static constexpr float BRICK_SPACING = 10.0f;

	int32_t _nbBricks = 0;
};