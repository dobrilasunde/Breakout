#include "Core/EngineModules/LevelEngineModule.h"
#include "Core/Level.h"

void LevelEngineModule::OnUpdate(float deltaSeconds)
{
	Super::OnUpdate(deltaSeconds);

	if (_nextLevel)
	{
		SwitchLevel();
	}
}

void LevelEngineModule::SwitchLevel()
{
	if (_currentLevel)
	{
		_currentLevel->OnDestroyed();
	}

	_currentLevel = std::move(_nextLevel);
	
	if (_currentLevel)
	{
		_currentLevel->OnStarted();
	}

	_nextLevel.reset();
}

void LevelEngineModule::OnLevelCreated(Level& level)
{
	level.OnCreated();
}
