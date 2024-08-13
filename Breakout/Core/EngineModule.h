#pragma once

class Window;

class EngineModule
{
public:
	virtual void OnUpdate(float deltaSeconds) {};
	virtual void OnDraw(Window& window) {};
};