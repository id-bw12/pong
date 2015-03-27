#ifndef GAMEHEADER_H
#define GAMEHEADER_H

#include "DeltaTime.h"
#include "ShapeHeader.h"
#include "Control.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class GameEntity: public ControlEntity
{
public:
	GameEntity(){};
	~GameEntity(){};
	void setupGame(bool twoPlayers);
	void setupPaddles(vector<ShapeEntity> &paddles, vector<sf::Vector2f> paddlePos);
	void setupLabels(sf::Text &scoreLabel, int &i);
	void playPong(GameState &currentState, sf::RenderWindow &window);
	void checkBounds(ShapeEntity &ball);
	void pushBackball(ShapeEntity &ball);
	void checkKeys(ShapeEntity &paddle);
	void lockPaddle(ShapeEntity &paddle);
	void ballCollison(ShapeEntity &ball, ShapeEntity &paddle);
	void checkAngle();
	void findCollisonPoint(ShapeEntity &ball, ShapeEntity &paddle);
	void drawLines(sf::RenderWindow &window, ShapeEntity ball, ShapeEntity paddle);
	void checkWinCondition(timer::Timer &timer);
	void checkPlayerInput(sf::RenderWindow &window, timer::Timer &timer, GameState &currentState);
	float findDistance(ShapeEntity ball, sf::Vector2f distancePoint);
	float findPoint(float value, float min, float max, float paddlePoint);

	
private:
	sf::Event m_event;
	sf::Vector2f m_distance, m_collisionPoint, m_direction;
	vector<ShapeEntity> paddles;
	vector<sf::Text> m_playScoreLabel;
	sf::Text winLabel;
	ShapeEntity m_gameBall;
	float m_velocity = 3.0f, m_paddleVelocity = 5.0f, m_ballAngle, m_length,
		m_top = 75.0f, m_bottom = 600.0f, m_left = 0.00f, m_right = 800.0f;

	const float  m_pi = 3.14159f;
	int m_playerOneScore = 0, m_playerTwoScore = 0;
	string winMessage = "";
};
#endif