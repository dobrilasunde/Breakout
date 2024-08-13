#include <iostream>

#include "SFML/Graphics/Text.hpp"

class Game
{
public:
	void Initialize();
	void RunLoop();

private:
	sf::Text _fpsText;

private:
	void CalculateFPS();
};