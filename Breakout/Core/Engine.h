#pragma once

#include "EngineModules/AssetEngineModule.h"
#include "EngineModules/InputEngineModule.h"
#include "EngineModules/LevelEngineModule.h"
#include "EngineModules/ObjectEngineModule.h"
#include "EngineModules/PhysicsEngineModule.h"
#include "Util/Proxy.h"

#include "SFML/Audio/Music.hpp"

#include <memory>
#include <type_traits>
#include <cassert>

class Level;
class Window;

class Engine
{
public:
	Engine();
	~Engine();

	void Initialize();

	void Update(float deltaSeconds);
	void Draw(Window& window);

	// Factory method for levels
	template<typename T>
	std::weak_ptr<T> StartLevel()
	{
		return _levelModule.StartLevel<T>();
	}

	template<typename T, typename... Args>
	ObjectHandle<T> CreateObject(Args&&... args)
	{
		return _objectModule.CreateObject<T>(std::forward<Args>(args)...);
	}

	void EnableCollisionForObject(const GameObject& object);
	void DisableCollisionForObject(const GameObject& object);

	void DestroyObject(const GameObject& object);

	bool GetInputActionState(EInputAction action) const;
	bool GetInputActionPressed(EInputAction action) const;
	bool GetInputActionReleased(EInputAction action) const;

	SFXSet& GetSFXSet();
	const sf::Font& GetFont() const;
	sf::Music& GetMusic();

private:
	InputEngineModule _inputModule;
	AssetEngineModule _assetModule;
	ObjectEngineModule _objectModule;
	PhysicsEngineModule _physicsModule;
	LevelEngineModule _levelModule;

private:
	void OnLevelCreated(Level& level);
	void OnLevelDestroyed(Level& level);
};

struct EngineProxy : public Proxy<Engine>
{
	EngineProxy();
};

namespace EngineStatics
{
	template<typename T, typename... Args>
	ObjectHandle<T> CreateObject(Args&&... args)
	{
		assert(EngineProxy::IsValid());
		return EngineProxy::Get().CreateObject<T>(std::forward<Args>(args)...);
	}
}