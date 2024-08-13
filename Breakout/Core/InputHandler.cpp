#include "Core/InputHandler.h"

#include "Core/Engine.h"

#include <cassert>

void InputHandler::ProcessInputs()
{
	assert(EngineProxy::IsValid());

	Engine& engine = EngineProxy::Get();
	for (const InputListener& listener : _inputListeners)
	{
		switch (listener.InputType)
		{
			case EInputType::Pressed:
			{
				if (engine.GetInputActionPressed(listener.InputAction))
				{
					listener.Callback();
				}

				break;
			}
			case EInputType::Released:
			{
				if (engine.GetInputActionPressed(listener.InputAction))
				{
					listener.Callback();
				}

				break;
			}
		}
	}
}
