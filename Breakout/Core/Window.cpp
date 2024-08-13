#include "Window.h"

#include "SFML/Window/Event.hpp"

#include <iostream>

sf::ContextSettings CreateWindowContextSettings()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	return settings;
}

Window::Window():
	_window(sf::VideoMode(WIDTH, HEIGHT), _gameTitle, sf::Style::Default, CreateWindowContextSettings())
{
	_window.setFramerateLimit(250);
	_window.setVerticalSyncEnabled(true);

	_window.setPosition(sf::Vector2i{ 100u, 0u });
	_window.setMouseCursorVisible(true);
}

void Window::Close()
{
	_window.close();
}

void Window::Clear(const sf::Color& color /*= {0, 0, 0}*/)
{
	_window.clear(color);
}

void Window::Display()
{
	_window.display();
}

void Window::PollEvents()
{
	sf::Event e;

	while (_window.pollEvent(e)) 
	{
		if (e.type == sf::Event::Closed) 
		{
			_window.close();
		}
		if (e.type == sf::Event::GainedFocus) 
		{
			_isInFocus = true;
		}
		if (e.type == sf::Event::LostFocus) 
		{
			_isInFocus = false;
		}
	}
}

void Window::Draw(const sf::Drawable& drawable)
{
	_window.draw(drawable);
}

void Window::SetMouseCursorVisible(bool value)
{
	_isMouseCursorVisible = value;
	_window.setMouseCursorVisible(value);
}

CurrentWindowProxy::CurrentWindowProxy() = default;