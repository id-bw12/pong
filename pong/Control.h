#ifndef CONTROL_H
#define CONTROL_H

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <vector>

class ControlEntity
{
public:
	
	ControlEntity(){m_font.loadFromFile("LinLibertine_R.ttf");};
	~ControlEntity(){};
	enum GameState {
		Uninitialized, WelcomeScreen, Paused,
		MainMenu, Playing, Exiting
	};

protected:
	//sf::RenderWindow m_window;
	sf::Font m_font;
};

#endif