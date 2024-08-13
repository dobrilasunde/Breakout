#pragma once

#include "InputActions.h"

#include <functional>

class InputHandler
{
public:
	using InputCallback = std::function<void()>;

public:
	void RegisterInputCallback(EInputAction action, EInputType type, const InputCallback& callback)
	{
		_inputListeners.emplace_back(action, type, callback);
	}

	template <typename Type>
	void RegisterInputCallback(Type* object, EInputAction action, EInputType type, void (Type::* callback)())
	{
		RegisterInputCallback(action, type, [object, callback]() { (object->*callback)(); });
	}

	void ProcessInputs();
	

private:
	struct InputListener
	{
		EInputAction InputAction;
		EInputType InputType;
		InputCallback Callback;

		InputListener(EInputAction action, EInputType inputType, const InputCallback& callback) :
			InputAction{ action },
			InputType{ inputType },
			Callback{ callback }
		{

		}
	};

private:
	std::vector<InputListener> _inputListeners;
};