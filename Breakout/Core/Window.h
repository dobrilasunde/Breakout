#include "Util/Proxy.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"

class Window
{
public:
	static constexpr uint16_t HEIGHT = 900;
	static constexpr uint16_t WIDTH = static_cast<uint16_t>(HEIGHT * 16.f / 9.f);

public:
	Window();

	void Close();

	void Clear(const sf::Color& color = {0, 0, 0});
	void Display();
	void PollEvents();

	bool IsOpen() const { return _window.isOpen(); }
	bool IsInFocus() const { return _isInFocus; }

	void Draw(const sf::Drawable& drawable);

	bool IsMouseCursorVisible() const { return _isMouseCursorVisible; }
	void SetMouseCursorVisible(bool value);

	const sf::View& GetView() { return _window.getView(); }
	void SetView(const sf::View& value) { _window.setView(value); }

	operator sf::RenderWindow&() { return _window; }
	operator const sf::RenderWindow&() const { return _window; }

private:
	sf::RenderWindow _window;
	const std::string _gameTitle = "Breakout";
	sf::View _view;
	
	bool _isInFocus = true;
	bool _isMouseCursorVisible = true;
};

struct CurrentWindowProxy : public Proxy<Window>
{
	CurrentWindowProxy();
};