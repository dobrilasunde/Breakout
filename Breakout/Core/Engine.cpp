#include "Engine.h"
#include "Level.h"

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::Initialize()
{
	_assetModule.LoadAllAssets();
}

void Engine::Update(float deltaSeconds)
{
	_inputModule.OnUpdate(deltaSeconds);
	_assetModule.OnUpdate(deltaSeconds);

	_objectModule.OnUpdate(deltaSeconds);
	_physicsModule.OnUpdate(deltaSeconds);
	_levelModule.OnUpdate(deltaSeconds);
}

void Engine::Draw(Window& window)
{
	_inputModule.OnDraw(window);
	_assetModule.OnDraw(window);
	_objectModule.OnDraw(window);
	_physicsModule.OnDraw(window);
	_levelModule.OnDraw(window);
}

void Engine::EnableCollisionForObject(const GameObject& object)
{
	ObjectHandle<GameObject> objectHandle = _objectModule.GetHandleForObject(object);

	_physicsModule.EnableCollisionForObject(objectHandle);
}

void Engine::DisableCollisionForObject(const GameObject& object)
{
	ObjectHandle<GameObject> objectHandle = _objectModule.GetHandleForObject(object);

	_physicsModule.DisableCollisionForObject(objectHandle);
}

void Engine::DestroyObject(const GameObject& object)
{
	ObjectHandle<GameObject> objectHandle = _objectModule.GetHandleForObject(object);

	_physicsModule.DisableCollisionForObject(objectHandle);
	_objectModule.DestroyObject(objectHandle);
}

bool Engine::GetInputActionState(EInputAction action) const
{
	return _inputModule.GetInputValue(action);
}

bool Engine::GetInputActionPressed(EInputAction action) const
{
	return _inputModule.GetInputPressed(action);
}

bool Engine::GetInputActionReleased(EInputAction action) const
{
	return _inputModule.GetInputReleased(action);
}

SFXSet& Engine::GetSFXSet()
{
	return _assetModule.GetSFXSet();
}

const sf::Font& Engine::GetFont() const
{
	return _assetModule.GetFont();
}

sf::Music& Engine::GetMusic()
{
	return _assetModule.GetMusic();
}

void Engine::OnLevelCreated(Level& level)
{
	level.OnCreated();
}

void Engine::OnLevelDestroyed(Level& level)
{
	level.OnDestroyed();
}

EngineProxy::EngineProxy() = default;