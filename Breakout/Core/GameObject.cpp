#include "GameObject.h"
#include "Core/Engine.h"

void GameObject::Update(float deltaSeconds)
{
	_inputHandler.ProcessInputs();
}

void GameObject::Destroy()
{
	if (EngineProxy::IsValid())
	{
		EngineProxy::Get().DestroyObject(*this);
	}
}

void GameObject::EnableCollision()
{
	if (_isCollisionEnabled)
	{
		return;
	}

	_isCollisionEnabled = true;

	if (EngineProxy::IsValid())
	{
		EngineProxy::Get().EnableCollisionForObject(*this);
	}
}

void GameObject::DisableCollision()
{
	if (!_isCollisionEnabled)
	{
		return;
	}

	_isCollisionEnabled = false;

	if (EngineProxy::IsValid())
	{
		EngineProxy::Get().DisableCollisionForObject(*this);
	}
}