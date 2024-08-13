#pragma once

#include "Core/EngineModule.h"

#include <memory>

class Level;

class LevelEngineModule : public EngineModule
{
	using Super = EngineModule;

public:
	// Level will be created immediately but started on the next frame.
	template<typename T>
	std::weak_ptr<T> StartLevel()
	{
		static_assert(std::is_base_of_v<Level, T> && "T must be level.");

		_nextLevel = std::make_shared<T>();
		OnLevelCreated(*_nextLevel.get());

		return std::dynamic_pointer_cast<T>(_nextLevel);
	}

	void OnUpdate(float deltaSeconds) override;

private:
	std::shared_ptr<Level> _currentLevel;
	std::shared_ptr<Level> _nextLevel;

private:
	void SwitchLevel();
	void OnLevelCreated(Level& level);
};
