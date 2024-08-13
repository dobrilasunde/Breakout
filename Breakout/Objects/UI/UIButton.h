#pragma once

#include "Objects/UI/UIObject.h"

#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class UIButton : public UIObject
{
	using Super = UIObject;

	static constexpr float FRAME_PADDING = 15.f;

public:
	using OnClickCallback = std::function<void()>;

	UIButton(const sf::Vector2f& position, int8_t updateOrder = 0):
		Super(position, updateOrder)
	{
		SetPosition(position);
	}

	void BeginPlay() override;
	void Draw(Window& window) override;

	void RegisterOnClickCallback(const OnClickCallback& callback)
	{
		_clickCallback = callback;
	}

	template <typename Type>
	void RegisterOnClickCallback(Type* object, void (Type::* callback)())
	{
		RegisterOnClickCallback([object, callback]() { (object->*callback)(); });
	}

protected:
	sf::FloatRect GetHoverBounds() const override;

private:
	OnClickCallback _clickCallback;

	sf::RectangleShape _frame;

private:
	void Input_OnClicked();
};