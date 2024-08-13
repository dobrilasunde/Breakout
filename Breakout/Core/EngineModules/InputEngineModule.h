#pragma once

#include "Core/EngineModule.h"
#include "Core/InputActions.h"

#include <bitset>

class InputEngineModule : public EngineModule
{
	using Super = EngineModule;
	using InputActionType = std::underlying_type_t<EInputAction>;
	using InputStates = std::bitset<static_cast<InputActionType>(EInputAction::Count)>;

public:
	void OnUpdate(float deltaSeconds) override;
	bool GetInputValue(EInputAction action) const { return _inputState[static_cast<InputActionType>(action)]; }
	bool GetInputPressed(EInputAction action) const { return _inputPressed[static_cast<InputActionType>(action)]; }
	bool GetInputReleased(EInputAction action) const { return _inputReleased[static_cast<InputActionType>(action)]; }

private:
	InputStates _inputState;
	InputStates _inputPressed;
	InputStates _inputReleased;

private:
	void SetInputValue(EInputAction action, bool value);
};