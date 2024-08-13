#include "UIObject.h"
#include "Core/Window.h"
#include "SFML/Window/Mouse.hpp"
#include "Core/Engine.h"

void UIObject::BeginPlay()
{
	_hoverSound.setBuffer(EngineProxy::Get().GetSFXSet()._menuButtonHover);
}

void UIObject::Update(float deltaSeconds)
{
	Super::Update(deltaSeconds);

	UpdateHover();
}

void UIObject::Draw(Window& window)
{
	Super::Draw(window);

	_text.setPosition(GetPosition());
	window.Draw(_text);
}

void UIObject::SetText(const sf::Text& text)
{
	_text = text; 
	
	const sf::Vector2f size = _text.getLocalBounds().getSize();
	_text.setOrigin(sf::Vector2f{ size.x * 0.5f, size.y });
}

sf::FloatRect UIObject::GetHoverBounds() const
{
	return GetText().getGlobalBounds();;
}

void UIObject::UpdateHover()
{
	const sf::Vector2i mousePos = sf::Mouse::getPosition(CurrentWindowProxy::Get());
	const sf::FloatRect& bounds = GetHoverBounds();

	const bool isHovered = bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	
	if (isHovered)
	{
		_text.setFillColor(_hoveredColor);
	}
	else
	{
		_text.setFillColor(_baseColor);
	}

	if (_playSoundOnHover && !_wasHovered && isHovered)
	{
		_hoverSound.play();
	}

	_wasHovered = isHovered;
}
