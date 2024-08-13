#pragma once

#include "ObjectsCommon.h"

#include <vector>

class GameObject;

class Level
{
public:
	virtual ~Level() = default;

	virtual void OnCreated() {}
	virtual void OnDestroyed();
	virtual void OnStarted() {}

	template <typename T>
	ObjectHandle<T> AddObjectToLevel(const ObjectHandle<T>& object)
	{
		const ObjectRef<T> objectRef = object.lock();
		objectRef->SetParentLevel(this);

		_levelObjects.emplace_back(object);
		return object;
	}
	
private:
	std::vector<ObjectHandle<GameObject>> _levelObjects;
};