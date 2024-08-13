#include "UIButton.h"

#include "Core/Window.h"

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Mouse.hpp"

void UIButton::BeginPlay()
{
	Super::BeginPlay();

	_frame.setFillColor(sf::Color::Transparent);
	_frame.setOutlineThickness(2.f);

	GetInputHandler().RegisterInputCallback(this, EInputAction::ButtonClick, EInputType::Pressed, &UIButton::Input_OnClicked);
}

void UIButton::Draw(Window& window)
{
	Super::Draw(window);

	GetText().setPosition(GetPosition());
	window.Draw(GetText());

	const sf::FloatRect textBounds = GetText().getGlobalBounds();
	_frame.setOutlineColor(WasHovered() ? GetHoveredColor() : GetBaseColor());
	_frame.setSize(textBounds.getSize() + sf::Vector2f{ FRAME_PADDING, FRAME_PADDING} * 2.f);
	_frame.setPosition(textBounds.getPosition() - sf::Vector2f{ FRAME_PADDING, FRAME_PADDING });
	window.Draw(_frame);
}

sf::FloatRect UIButton::GetHoverBounds() const
{
	return sf::FloatRect(_frame.getPosition().x, _frame.getPosition().y, _frame.getSize().x, _frame.getSize().y);
}

void UIButton::Input_OnClicked()
{
	const sf::Vector2i mousePos = sf::Mouse::getPosition(CurrentWindowProxy::Get());
	const sf::FloatRect& bounds = GetHoverBounds();

	if (bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
	{
		_clickCallback();
	}
}
