#include "ObjectEngineModule.h"
#include "Core/GameObject.h"

#include <algorithm>

ObjectEngineModule::ObjectEngineModule() = default;

void ObjectEngineModule::OnUpdate(float deltaSeconds)
{
	Super::OnUpdate(deltaSeconds);

	HandleObjectsPendingBeginPlay();

	for (const ObjectRef<GameObject>& object : _objects)
	{
		object->Update(deltaSeconds);
	}
	
	HandleObjectsPendingDestroy();
}

void ObjectEngineModule::OnDraw(Window& window)
{
	Super::OnDraw(window);
	
	for (const ObjectRef<GameObject>& object : _objects)
	{
		object->Draw(window);
	}
}

ObjectHandle<GameObject> ObjectEngineModule::GetHandleForObject(const GameObject& object)
{
	auto it = std::find_if(_objects.begin(), _objects.end(), [&object](const ObjectRef<GameObject>& oRef) {

			return oRef.get() == &object;
		});

	if (it != _objects.end())
	{
		return *it;
	}

	return {};
}

void ObjectEngineModule::OnObjectCreated()
{
	std::sort(_objects.begin(), _objects.end(), [](const ObjectRef<GameObject>& lhs, const ObjectRef<GameObject>& rhs) { return lhs->GetUpdateOrder() < rhs->GetUpdateOrder(); });
}

void ObjectEngineModule::HandleObjectsPendingBeginPlay()
{
	if (_objectsPendingBeginPlay.size() == 0)
	{
		return;
	}

	std::vector<ObjectHandle<GameObject>> objectsPendingBeginPlayCopy = _objectsPendingBeginPlay;
	_objectsPendingBeginPlay.clear();

	for (const ObjectHandle<GameObject>& object : objectsPendingBeginPlayCopy)
	{
		ObjectRef<GameObject> objectRef = object.lock();
		objectRef->BeginPlay();
	}
}

void ObjectEngineModule::HandleObjectsPendingDestroy()
{
	if (!_objectsPendingDestroy.empty())
	{
		std::vector<ObjectHandle<GameObject>>& objectsPendingDestroy = _objectsPendingDestroy;

		auto CheckObjectInPendingDestroy = [&objectsPendingDestroy](const ObjectHandle<GameObject>& inObject)
			{
				const ObjectRef<GameObject> inObjectRef = inObject.lock();

				auto it = std::remove_if(objectsPendingDestroy.begin(), objectsPendingDestroy.end(), [&inObjectRef](const ObjectHandle<GameObject>& o) { return o.lock() == inObjectRef; });

				const bool objectInDestructionQueue = it != objectsPendingDestroy.end();
				if (objectInDestructionQueue)
				{
					inObjectRef->EndPlay();
					objectsPendingDestroy.erase(it, objectsPendingDestroy.end());
				}

				return objectInDestructionQueue;
			};

		_objects.erase(std::remove_if(_objects.begin(), _objects.end(), CheckObjectInPendingDestroy), _objects.end());
	}
}
