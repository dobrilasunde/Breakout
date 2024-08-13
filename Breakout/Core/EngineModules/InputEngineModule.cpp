#include "InputEngineModule.h"

#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/Keyboard.hpp"

void InputEngineModule::OnUpdate(float deltaSeconds)
{
	Super::OnUpdate(deltaSeconds);

	InputStates lastInputState = _inputState;
	_inputState.reset();
	_inputPressed.reset();
	_inputReleased.reset();

	// Input mapping
	SetInputValue(EInputAction::LaunchBall, sf::Mouse::isButtonPressed(sf::Mouse::Left));
	SetInputValue(EInputAction::ButtonClick, sf::Mouse::isButtonPressed(sf::Mouse::Left));
	SetInputValue(EInputAction::Pause, sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));
	SetInputValue(EInputAction::EnableDebug, sf::Keyboard::isKeyPressed(sf::Keyboard::D));

	for (InputActionType i = 0; i < static_cast<InputActionType>(EInputAction::Count); ++i)
	{
		bool lastState = lastInputState[i];
		bool newState = _inputState[i];

		_inputPressed[i] = !lastState && newState;
		_inputReleased[i] = lastState && !newState;
	}
}

void InputEngineModule::SetInputValue(EInputAction action, bool value)
{
	using ActionType = std::underlying_type_t<EInputAction>;

	if (value)
	{
		_inputState.set(static_cast<ActionType>(action));
	}
	else
	{
		_inputState.reset(static_cast<ActionType>(action));
	}
}
