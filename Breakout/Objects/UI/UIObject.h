#pragma once

#include "Core/GameObject.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Audio/Sound.hpp"

class UIObject : public GameObject
{
	using Super = GameObject;

public:
	UIObject(const sf::Vector2f& position, int8_t updateOrder = 0):
		Super(updateOrder)
	{
		SetPosition(position);
	}

	void BeginPlay() override;
	void Update(float deltaSeconds) override;
	void Draw(Window& window) override;

	void SetText(const sf::Text& text);
	void SetBaseColor(const sf::Color& value) { _baseColor = value; }
	void SetHoveredColor(const sf::Color& value) { _hoveredColor = value;}

	void SetPlaySoundOnHover(bool value) { _playSoundOnHover = value; }

	const sf::Color& GetBaseColor() const { return _baseColor; }
	const sf::Color& GetHoveredColor() const { return _hoveredColor; }

	bool WasHovered() const { return _wasHovered; }

protected:
	sf::Text& GetText() { return _text; }
	const sf::Text& GetText() const { return _text; }

	virtual sf::FloatRect GetHoverBounds() const;

private:
	sf::Text _text;
	sf::Color _baseColor = sf::Color::White;
	sf::Color _hoveredColor = sf::Color::White;
	sf::Sound _hoverSound;
	bool _playSoundOnHover = false;

	bool _wasHovered = false;

private:
	void UpdateHover();
};