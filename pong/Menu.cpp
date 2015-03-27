#include "menuClass.h"
#include "ShapeHeader.h"
#include "Control.h"
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <functional>

using namespace std;

MenuClass::MenuClass()
{
	m_names = { "PlayButton", "OptionsButton", "CreditsButton", "ExitButton",
		"CompButton", "2PlayerButton", "IDButton", "MediaButton", "ResourcesButton", "BackButton" };

	if (m_currentState != Uninitialized)
		return;

	m_window.create(sf::VideoMode(800, 600), "testing");

	m_currentState = GameState::WelcomeScreen;

	m_menuItems.emplace(m_names[0], bind(&MenuClass::playerSelect, this));
	m_menuItems.emplace(m_names[1], bind(&MenuClass::pongOptions, this));
	m_menuItems.emplace(m_names[2], bind(&MenuClass::creditsInformation, this));
	m_menuItems.emplace(m_names[3], bind(&MenuClass::exitGame, this));
	m_menuItems.emplace(m_names[4], bind(&MenuClass::pongGame, this, false));
	m_menuItems.emplace(m_names[5], bind(&MenuClass::pongGame, this, true));

}

MenuClass::~MenuClass()
{

}

void MenuClass::startPong()
{
	while (m_currentState != Exiting)
	{
		switch (m_currentState)
		{

		case WelcomeScreen:
			welcomeScreen();
			break;

		case MainMenu:
			mainMenu();
			break;

		default:
			break;
		}
	}
}

void MenuClass::welcomeScreen()
{
	sf::Text text;

	text.setFont(ControlEntity::m_font);
	text.setString("Hit anything to enter.");
	text.setCharacterSize(24);
	text.setColor(sf::Color::White);
	text.setPosition(m_window.getSize().x / 3.f, m_window.getSize().y / 2.f);

	m_window.draw(text);
	m_window.display();

	while (m_currentState == WelcomeScreen)
	{
		if (m_window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
				m_currentState = Exiting;
			else
				if (m_event.type == sf::Event::KeyPressed)
					m_currentState = MainMenu;

		}
	}

	m_window.clear();
}

void MenuClass::mainMenu()
{
	vector<Button> button =  vector<Button>();
	
	vector<sf::Vector2f> buttonPos = { sf::Vector2f(300, 200), sf::Vector2f(300, 260),
		sf::Vector2f(300, 320), sf::Vector2f(300, 380) };

	sf::Event event;

	vector<string> menuText = { "Play Pong", "Options", "Id Info", "Exit Game" };
	vector<string> buttonName = vector<string>(m_names.begin(), m_names.begin() + m_names.size() / 2);

	vector<Label> label = vector<Label>();
	vector<sf::Vector2f> LabelPos = { sf::Vector2f(250, 100) };
	vector<string> labelName = {"WelcomeLabel"};

	auto xStart = 0.f, yStart = 0.f, xEnd = 0.f, yEnd = 0.f;

	string mapKey = "";

	buttonSetup(buttonPos, button, 4, menuText, buttonName, ControlEntity::m_font);

	textLabelSetup(label, sf::Color::White, LabelPos, ControlEntity::m_font, labelName);

	displayUI(button, label, m_window);

	while (m_currentState == GameState::MainMenu)
	{
		if (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_currentState = GameState::Exiting;
			else
				if (event.type == sf::Event::MouseButtonReleased)
				{
					for (auto iter = button.begin(); iter != button.end(); ++iter)
					{
						xStart = (*iter).button.getPosition().x;
						yStart = (*iter).button.getPosition().y;
						xEnd = (*iter).button.getPosition().x + (*iter).button.getSize().x;
						yEnd = (*iter).button.getPosition().y + (*iter).button.getSize().y;
						if (event.mouseButton.x >= xStart && event.mouseButton.y >= yStart &&
							event.mouseButton.x <= xEnd && event.mouseButton.y <= yEnd){

							mapKey = iter->buttonName;
						}
					}
					if (mapKey != "")
					{
						m_menuItems[mapKey]();
						mapKey = "";
						displayUI(button, label, m_window);
					}
					
				}
					
		}


	}

}

void MenuClass::playerSelect()
{
	vector<Button> button = vector<Button>();

	vector<string> buttonText = { "Vs Comp", "2 Players", "Back" };
	vector<string> buttonName = vector<string>(m_names.begin() + 4, m_names.begin() + 6);

	vector<sf::Vector2f> buttonPos = { sf::Vector2f(160, 450), sf::Vector2f(325, 450),
		sf::Vector2f(495, 450) };

	sf::Event event;

	vector<Label> label = vector<Label>();
	
	string mapKey = "";

	auto xStart = 0.f, yStart = 0.f, xEnd = 0.f, yEnd = 0.f;

	bool backLevel = false;

	buttonName.push_back(m_names[9]);

	buttonSetup(buttonPos, button, 3, buttonText, buttonName, m_font);

	displayUI(button, label, m_window);

	while (!backLevel && m_window.isOpen())
	{
		if (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_currentState = GameState::Exiting;
				backLevel = true;
			}
			else
				if (event.type == sf::Event::MouseButtonReleased)
					for (auto iter = button.begin(); iter != button.end(); ++iter)
					{
						xStart = (*iter).button.getPosition().x;
						yStart = (*iter).button.getPosition().y;
						xEnd = (*iter).button.getPosition().x + (*iter).button.getSize().x;
						yEnd = (*iter).button.getPosition().y + (*iter).button.getSize().y;
						if (event.mouseButton.x >= xStart && event.mouseButton.y >= yStart &&
							event.mouseButton.x <= xEnd && event.mouseButton.y <= yEnd)
						{
							if (iter->buttonName == buttonName[2])
								backLevel = true;
							else
								mapKey = iter->buttonName;
						}
						
					}
			if (mapKey != "")
			{
				m_menuItems[mapKey]();
				mapKey = "";
				displayUI(button, label, m_window);
			}
		}
	}


}

void MenuClass::pongGame(const bool &twoPlayers)
{
	//m_window.setSize(sf::Vector2u(900, 600));
	GameEntity *pong = new GameEntity();
	pong->setupGame(twoPlayers);
	pong->playPong(m_currentState, m_window);

	delete pong;
	pong = nullptr;
}

void MenuClass::pongOptions()
{
	vector<Button> button = vector<Button>();

	vector<string> buttonText = { "Back" };
	vector<string> buttonName = vector<string>(m_names.begin() + 9, m_names.end());

	vector<sf::Vector2f> buttonPos = { sf::Vector2f(300, 500) };

	sf::Event event;

	vector<Label> label = vector<Label>();

	auto xStart = 0.f, yStart = 0.f, xEnd = 0.f, yEnd = 0.f;

	bool backLevel = false;

	buttonSetup(buttonPos, button, 1, buttonText, buttonName, m_font);

	displayUI(button, label, m_window);

	while (!backLevel)
	{
		if (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_currentState = GameState::Exiting;
				backLevel = true;
			}
			else
				if (event.type == sf::Event::MouseButtonReleased)
					for (auto iter = button.begin(); iter != button.end(); ++iter)
					{
						xStart = (*iter).button.getPosition().x;
						yStart = (*iter).button.getPosition().y;
						xEnd = (*iter).button.getPosition().x + (*iter).button.getSize().x;
						yEnd = (*iter).button.getPosition().y + (*iter).button.getSize().y;
						if (event.mouseButton.x >= xStart && event.mouseButton.y >= yStart &&
							event.mouseButton.x <= xEnd && event.mouseButton.y <= yEnd)
						{
								backLevel = true;
						}
					}
		}
	}
}

void MenuClass::creditsInformation()
{
	vector<Button> button = vector<Button>();

	vector<string> buttonText = { "Id Info","Media","Resources","Back" };
	vector<string> buttonName = vector<string>(m_names.begin() + 6, m_names.end());

	vector<sf::Vector2f> buttonPos = { sf::Vector2f(80, 450), sf::Vector2f(245, 450),
		sf::Vector2f(410, 450), sf::Vector2f(575, 450) };

	sf::Event event;

	vector<Label> label = vector<Label>();

	auto xStart = 0.f, yStart = 0.f, xEnd = 0.f, yEnd = 0.f;

	bool backLevel = false;

	buttonSetup(buttonPos, button, 4, buttonText, buttonName, m_font);

	displayUI(button, label, m_window);

	while (!backLevel)
	{
		if (m_window.pollEvent(event))
		{
			if (m_event.type == sf::Event::Closed)
			{
				m_currentState = GameState::Exiting;
				backLevel = true;
			}
			else
				if (event.type == sf::Event::MouseButtonReleased)
					for (auto iter = button.begin(); iter != button.end(); ++iter)
					{
						xStart = (*iter).button.getPosition().x;
						yStart = (*iter).button.getPosition().y;
						xEnd = (*iter).button.getPosition().x + (*iter).button.getSize().x;
						yEnd = (*iter).button.getPosition().y + (*iter).button.getSize().y;
						if (event.mouseButton.x >= xStart && event.mouseButton.y >= yStart &&
							event.mouseButton.x <= xEnd && event.mouseButton.y <= yEnd)
						{
							if (iter->buttonName == buttonName[3])
								backLevel = true;
						}
					}
		}
	}
}

void MenuClass::exitGame()
{
	m_currentState = GameState::Exiting;
}

