#ifndef MENUCLASS_H
#define MENUCLASS_H

#include "Control.h"
#include "GameHeader.h"
#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

using namespace std;

class UIClass
{

private:
	struct ButtonStruct
	{
		sf::RectangleShape button;
		sf::Text buttonText;
		sf::Font font;
		string buttonName = "";
	};
	struct LabelStruct
	{
		sf::Text labeltext;
		sf::Font labelFont;
		string labelName = "";
	};

public:
	typedef ButtonStruct Button;
	typedef LabelStruct Label;

public:
	UIClass(){};
	~UIClass(){};

	void buttonSetup(vector<sf::Vector2f> buttonPosition,
		vector<Button> &button, int size, vector<string> buttonText, vector<string> name,
		sf::Font font);

	void textLabelSetup(vector<Label> &labelText, sf::Color color, vector<sf::Vector2f> labelPos,
		sf::Font font, vector<string> labelName);

	void displayUI(vector<Button> button, vector<Label> label, sf::RenderWindow &window);


};


class MenuClass : public ControlEntity, UIClass
{
public:
	MenuClass();
	~MenuClass();
	void startPong();
	void welcomeScreen();
	void mainMenu();
	void playerSelect();
	void pongGame(const bool &twoPlayers);
	void pongOptions();
	void creditsInformation();
	void exitGame();

private:
	GameState m_currentState = GameState::Uninitialized;

	sf::RenderWindow m_window;
	sf::Event m_event;
	//sf::Font m_font;

	vector<string> m_names;

	unordered_map<string, function<void()>> m_menuItems;
	function<void()> items;
	bool m_secondPlayers = true, m_computer = false;
};

#endif