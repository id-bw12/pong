#include "menuClass.h"
#include <iostream>
#include <iterator>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

void UIClass::displayUI(vector<Button> button, vector<Label> label, sf::RenderWindow &window)
{
	window.clear();

	for (Button but : button)
	{
		window.draw(but.button);
		but.buttonText.setFont(but.font);
		window.draw(but.buttonText);
	}

	for (Label label : label)
	{
		label.labeltext.setFont(label.labelFont);
		window.draw(label.labeltext);
	}
		

	window.display();
}

void UIClass::buttonSetup(vector<sf::Vector2f> buttonPosition, vector<Button> &button,
	int size, vector<string> buttonText, vector<string> name, sf::Font font)
{
	Button tempButton = Button();

	float x = 0, y = 0;

	tempButton.font = font;

	tempButton.button.setSize(sf::Vector2f(150,50));
	tempButton.button.setOutlineThickness(2);
	tempButton.button.setOutlineColor(sf::Color::White);
	tempButton.button.setFillColor(sf::Color::Red);

	tempButton.buttonText.setColor(sf::Color::Blue);
	tempButton.buttonText.setFont(tempButton.font);

	button.reserve(size);
	button.assign(size, tempButton);

	auto iterPos = buttonPosition.begin();
	auto iterText = buttonText.begin();
	auto iterName = name.begin();
	for (auto iter = button.begin(); iter != button.end(); ++iter)
	{
		
		iter->button.setPosition(*iterPos++);
		iter->buttonName = *iterName++;
		x = iter->button.getPosition().x + (iter->button.getGlobalBounds().width/ 2);
		y = iter->button.getPosition().y + (iter->button.getGlobalBounds().height / 4);

		iter->buttonText.setString(*iterText++);
		iter->buttonText.setOrigin(iter->buttonText.getGlobalBounds().width / 2, iter->buttonText.getGlobalBounds().height / 3);
		iter->buttonText.setPosition(sf::Vector2f(x, y));
	}

}

void UIClass::textLabelSetup(vector<Label> &labelText, sf::Color color, vector<sf::Vector2f> labelPos,
	sf::Font font, vector<string> labelName)
{
	Label tempLabel = Label();

	tempLabel.labelFont = font;

	tempLabel.labeltext.setFont(font);
	tempLabel.labeltext.setColor(color);
	tempLabel.labeltext.setOrigin(tempLabel.labeltext.getGlobalBounds().width / 2, tempLabel.labeltext.getGlobalBounds().height / 2 );
	tempLabel.labeltext.setPosition(labelPos[0]);
	tempLabel.labelName = labelName[0];
	tempLabel.labeltext.setString("Welcome to Pong.");

	labelText.reserve(1);
	labelText.assign(1, tempLabel);

}