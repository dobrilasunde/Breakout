#pragma once

#include "Core/EngineModule.h"
#include "Core/ObjectsCommon.h"

#include <memory>
#include <vector>

class GameObject;
class Window;

class ObjectEngineModule : public EngineModule
{
	using Super = EngineModule;

public:
	ObjectEngineModule();

	template<typename T, typename... Args>
	ObjectHandle<T> CreateObject(Args&&... args)
	{
		static_assert(std::is_base_of_v<GameObject, T> && "T must be a GameObject.");
		ObjectRef<GameObject> newObject = _objects.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
		OnObjectCreated();

		_objectsPendingBeginPlay.emplace_back(newObject);

		ObjectHandle<T> r = std::dynamic_pointer_cast<T>(newObject);
		return r;
	}

	void OnUpdate(float deltaSeconds) override;
	void OnDraw(Window& window) override;

	ObjectHandle<GameObject> GetHandleForObject(const GameObject& object);

	void DestroyObject(ObjectHandle<GameObject> object)
	{
		_objectsPendingDestroy.emplace_back(object);
	}

private:
	std::vector<ObjectRef<GameObject>> _objects;
	std::vector<ObjectHandle<GameObject>> _objectsPendingBeginPlay;
	std::vector<ObjectHandle<GameObject>> _objectsPendingDestroy;

private:
	void OnObjectCreated();

	void HandleObjectsPendingBeginPlay();
	void HandleObjectsPendingDestroy();
};