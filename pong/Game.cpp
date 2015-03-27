#include "GameHeader.h"
#include <SFML\Graphics.hpp>
#include <windows.h>
#include <cmath>
#include <iostream>
#include <functional>
#include <vector>

//http://www.wildbunny.co.uk/blog/2011/04/20/collision-detection-for-dummies/
using namespace std;
using namespace std::placeholders;
using namespace timer;

void GameEntity::setupGame(bool twoPlayers)
{
	vector<sf::Vector2f> paddlePos = { { 100, 400 }, { 700, 400 } };
	int i = 0;

	paddles = vector<ShapeEntity>();

	paddles.push_back(ShapeEntity(new sf::RectangleShape(), nullptr, "PlayerOnePaddle"));

	if (twoPlayers)
		paddles.push_back(ShapeEntity(new sf::RectangleShape(), nullptr, "PlayerTwoPaddle"));
	else
		paddles.push_back(ShapeEntity(new sf::RectangleShape(), nullptr, "CPUPaddle"));


	setupPaddles(paddles, paddlePos);

	m_gameBall = ShapeEntity(nullptr, new sf::CircleShape(), "Ball");

	m_gameBall.setShapeColor(sf::Color::Magenta);
	m_gameBall.setPosition(sf::Vector2f(400, 400));
	m_gameBall.setRadius(25);
	m_gameBall.setupShape();

	m_playScoreLabel.reserve(2);
	m_playScoreLabel.assign(2 ,sf::Text());

	for_each(m_playScoreLabel.begin(), m_playScoreLabel.end(), bind(&GameEntity::setupLabels, this, _1, i));

	m_playScoreLabel[0].setString("Player 1 Score: " + to_string(m_playerOneScore));
	m_playScoreLabel[1].setString("Player 2 Score: " + to_string(m_playerTwoScore));

	m_direction = sf::Vector2f(1, -1); 
}

void GameEntity::setupLabels(sf::Text &scoreLabel, int &i)
{
	vector<sf::Vector2f> position = { sf::Vector2f(10, 10), sf::Vector2f(550, 10) };

	scoreLabel.setFont(ControlEntity::m_font);
	scoreLabel.setColor(sf::Color::White);
	scoreLabel.setPosition(position[i]);
	i++;
}

void GameEntity::setupPaddles(vector<ShapeEntity> &paddles, vector<sf::Vector2f> paddlePos)
{
	for (int i = 0; i < 2; i++)
	{
		paddles[i].setShapeColor(sf::Color::Green);
		paddles[i].setSize(sf::Vector2f(25, 75));
		paddles[i].setBorderColor(sf::Color::White);
		paddles[i].setPosition(paddlePos[i]);
		paddles[i].setupShape();
	}
}

void GameEntity::playPong(GameState &currentState, sf::RenderWindow &window)
{
	sf::Vector2f ballPos;

	timer::Timer timerOne = Timer(true);

	srand(time(NULL));
	
	m_ballAngle = rand() % 360 + 0;

	window.clear();
	window.draw(paddles[0].getPaddle());
	window.draw(paddles[1].getPaddle());
	window.draw(m_gameBall.getBall());
	window.draw(m_playScoreLabel[0]);
	window.draw(m_playScoreLabel[1]);
	window.display();

	timerOne.setTimeLimit(.015f);

	currentState = Playing;

	while (currentState != Exiting && currentState != MainMenu)
	{
		if (window.pollEvent(m_event))
		{
			if (m_event.type == sf::Event::Closed)
			{
				currentState = GameState::Exiting;
				window.close();
			}
			if (m_event.type == sf::Event::KeyPressed && currentState != MainMenu)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && currentState != Paused)
					currentState = Paused;
				else
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && currentState != Playing && currentState != Exiting)
						currentState = Playing;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					currentState = MainMenu;
			}
			
		}

		if (m_velocity > 8)
			m_velocity = 8;

		if (timerOne.getPowerSwitch())
		{
			if (timerOne.checkDelta())
			{
				if (currentState != Paused && currentState != Exiting)
				{
					for_each(paddles.begin(), paddles.end(), bind(&GameEntity::checkKeys, this, _1));

					ballPos = sf::Vector2f(cos(m_ballAngle), sin(m_ballAngle));

					ballPos.x /= sqrt(ballPos.x*ballPos.x + ballPos.y*ballPos.y);
					ballPos.y /= sqrt(ballPos.x*ballPos.x + ballPos.y*ballPos.y);

					ballPos.x += m_velocity*m_direction.x;
					ballPos.y += m_velocity*m_direction.y;

					m_gameBall.moveShape(ballPos);
					checkBounds(m_gameBall);
					for_each(paddles.begin(), paddles.end(), bind(&GameEntity::ballCollison, this, m_gameBall, _1));

					m_playScoreLabel[0].setString("Player 1 Score: " + to_string(m_playerOneScore));
					m_playScoreLabel[1].setString("Player 2 Score: " + to_string(m_playerTwoScore));

					window.clear();
					window.draw(m_gameBall.getBall());
					window.draw(paddles[0].getPaddle());
					window.draw(paddles[1].getPaddle());
					window.draw(m_playScoreLabel[0]);
					window.draw(m_playScoreLabel[1]);
					/*drawLines(window, m_gameBall, paddles[0]);
					drawLines(window, m_gameBall, paddles[1]);*/
					window.display();

					checkWinCondition(timerOne);

				}
				timerOne.resetStartTime();
			}

		}
		else
		{
			window.clear();
			window.draw(m_gameBall.getBall());
			window.draw(paddles[0].getPaddle());
			window.draw(paddles[1].getPaddle());
			window.draw(m_playScoreLabel[0]);
			window.draw(m_playScoreLabel[1]);
			window.draw(winLabel);
			window.display();

			checkPlayerInput(window, timerOne, currentState);
		}

			
	}
}

void GameEntity::checkBounds(ShapeEntity &ball)
{
	float top = 0.00f, bottom = 600.00f, left = 0.00f, right = 800.00f;
	sf::Vector2f prevPosisition;

	float leftBall = ball.getPosition().x - ball.getRadius(),
		rightBall = ball.getPosition().x + ball.getRadius(),
		topBall = ball.getPosition().y - ball.getRadius(),
		bottomBall = ball.getPosition().y + ball.getRadius();

	if ((topBall < m_top) || (bottomBall > m_bottom))
	{
		prevPosisition = ball.getPosition();
		pushBackball(ball);
	}

	if (leftBall < m_left || (rightBall > m_right))
	{
		if (leftBall < m_left)
			m_playerTwoScore += 1;
		else
			if (rightBall > m_right)
				m_playerOneScore += 1;

		ball.setPosition(sf::Vector2f(400, 200));
		m_velocity = 3;
		m_ballAngle = 360.f - m_ballAngle;
	}
	


	if (ball.getPosition().y + ball.getRadius() < m_top)
		ball.setPosition(sf::Vector2f(ball.getPosition().x, m_top + 1));
	else
		if (ball.getPosition().y - ball.getRadius() > m_bottom)
			ball.setPosition(sf::Vector2f(ball.getPosition().x, m_bottom - 1));

	
}

void GameEntity::pushBackball(ShapeEntity &ball)
{
	float newY = 0.00f;
	sf::Vector2f position;

	if ((ball.getBall().getPosition().y - ball.getRadius()) < m_top){
		newY = (ball.getBall().getPosition().y - ball.getRadius() - m_top) + m_velocity;
		position = (sf::Vector2f(ball.getBall().getPosition().x, ball.getBall().getPosition().y + newY));
		m_direction.y *= -1;
	}
	else
		if ((ball.getBall().getPosition().y + ball.getRadius()) > m_bottom){
			newY = (ball.getBall().getPosition().y + ball.getRadius() - m_bottom) + m_velocity;
			position = (sf::Vector2f(ball.getBall().getPosition().x, ball.getPosition().y - newY));
			m_direction.y *= -1;
		}

	m_ballAngle = 360.f - m_ballAngle;

	m_velocity += 1;
	ball.setPosition(position);
}

void GameEntity::checkAngle()
{
	if (m_ballAngle < 0)
		m_ballAngle = 360.f + m_ballAngle;
}

void GameEntity::checkKeys(ShapeEntity &paddle)
{
	if (paddle.getName() == "PlayerOnePaddle")
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			paddle.moveShape(sf::Vector2f(0,m_paddleVelocity));
		else
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				paddle.moveShape(sf::Vector2f(0, -m_paddleVelocity));

	}
	else
		if (paddle.getName() == "PlayerTwoPaddle")
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				paddle.moveShape(sf::Vector2f(0, m_paddleVelocity));
			else
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					paddle.moveShape(sf::Vector2f(0, -m_paddleVelocity));
		}

	lockPaddle(paddle);
}

void GameEntity::lockPaddle(ShapeEntity &paddle)
{
	sf::Vector2f position;
	float yTop = paddle.getPosition().y - (paddle.getHeight() / 2),
		yBot = paddle.getPosition().y + (paddle.getHeight() / 2);

	if (yTop < m_top)
	{
		float yNew = paddle.getPosition().y - (paddle.getHeight()/2) - m_top;
		position = (sf::Vector2f(paddle.getPosition().x, paddle.getPosition().y - yNew));
		paddle.setPosition(position);
	}
	else
		if (yBot > m_bottom)
		{
			float yNew = paddle.getPosition().y + (paddle.getHeight() / 2) - m_bottom;
			position = (sf::Vector2f(paddle.getPosition().x, paddle.getPosition().y - yNew));
			paddle.setPosition(position);
		}
	
}

void GameEntity::ballCollison(ShapeEntity &ball, ShapeEntity &paddle)
{

	sf::Vector2f pushback, distanceTocollision;
	float length = 0;

	findCollisonPoint(ball,paddle);

	if (ball.getPosition().x < paddle.getPosition().x)
		distanceTocollision = m_collisionPoint - ball.getPosition();
	else
		distanceTocollision = ball.getPosition() - m_collisionPoint;

	length = (sqrt(distanceTocollision.x*distanceTocollision.x + distanceTocollision.y*distanceTocollision.y) - ball.getRadius());

	if (length < 0)
	{
	
		if (ball.getPosition().y == m_collisionPoint.y)
		{
			ball.setPosition(sf::Vector2f(ball.getPosition().x + (length*m_direction.x), ball.getPosition().y));
			m_direction.x *= -1;
		}
		else
			if (ball.getPosition().x == m_collisionPoint.x)
			{
				ball.setPosition(sf::Vector2f(ball.getPosition().x, ball.getPosition().y + (length*m_direction.y)));
				m_direction.y *= -1;
				m_direction.x *= -1; 
			}
			else
				if (ball.getPosition().y != m_collisionPoint.y && ball.getPosition().x != m_collisionPoint.x)
				{
					ball.setPosition(sf::Vector2f(ball.getPosition().x + (length*m_direction.x), ball.getPosition().y + (length*m_direction.y)));
					m_direction.x *= -1;
				}
				
		m_velocity += 1;
		m_ballAngle = 360 - m_ballAngle;
	}

}

void GameEntity::drawLines(sf::RenderWindow &window, ShapeEntity ball, ShapeEntity paddle)
{
	sf::RectangleShape bline;
	
	findCollisonPoint(ball, paddle);
	bline.setPosition(m_collisionPoint.x, m_collisionPoint.y);
	bline.setSize(sf::Vector2f(1, m_length));
	if (ball.getPosition().x <= paddle.getPosition().x)
		bline.setOrigin(0.f, 0.f);
	else
		if (ball.getPosition().x >= paddle.getPosition().x)
			bline.setOrigin(0.f, bline.getSize().y);

	//90 degrees - theta
	bline.rotate(90.f - (asin(m_distance.y / m_length)*180.00f) / m_pi);

	bline.setFillColor(sf::Color::Blue);

	window.draw(bline);

}

void GameEntity::findCollisonPoint(ShapeEntity &ball, ShapeEntity &paddle)
{
	float xMin = paddle.getPosition().x - (paddle.getWidth() / 2),
		xMax = paddle.getPosition().x + (paddle.getWidth() / 2),
		yMin = paddle.getPosition().y - (paddle.getHeight() / 2),
		yMax = paddle.getPosition().y + (paddle.getHeight() / 2);

	m_collisionPoint.x = findPoint(ball.getPosition().x, xMin, xMax, paddle.getPosition().x);
	m_collisionPoint.y = findPoint(ball.getPosition().y, yMin, yMax, paddle.getPosition().y);

	m_length = findDistance(ball, m_collisionPoint);
}

void GameEntity::checkWinCondition(timer::Timer &timer)
{
	if (m_playerOneScore == 10 || m_playerTwoScore == 10)
	{

		if (m_playerOneScore == 10)
			winMessage = "Player One Wins. Would you like to play again.\nHit Y for Yes or N for No";
		else
			if (m_playerTwoScore == 10)
				winMessage = "Player Two Wins. Would you like to play again.\nHit Y for Yes or N for No";

		winLabel.setString(winMessage);
		winLabel.setFont(m_font);
		winLabel.setColor(sf::Color::White);
		winLabel.setPosition(sf::Vector2f(150, 200));
		timer.offAndOnSwitch(false);

	}
	
}

void GameEntity::checkPlayerInput(sf::RenderWindow &window, timer::Timer &timer, GameState &currentState)
{
	bool input = true;
	sf::Event event;

	while (input)
	{
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				currentState = GameState::Exiting;
				window.close();
				input = false;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					m_playerOneScore = 0;
					m_playerTwoScore = 0;
					timer.offAndOnSwitch(true);
					timer.resetStartTime();
					input = false;
					m_direction = sf::Vector2f(1, -1);
				}
				else
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
					{
						input = false;
						currentState = MainMenu;
					}
			}
		}

	}
}

float GameEntity::findDistance(ShapeEntity ball, sf::Vector2f distancePoint)
{
	float length;

	if (ball.getPosition().x < distancePoint.x)
		m_distance = ball.getPosition() - distancePoint;
	else
		if (distancePoint.x < ball.getPosition().x)
			m_distance = distancePoint - ball.getPosition();
		else
			m_distance = ball.getPosition() - distancePoint;


	length = sqrt(m_distance.x*m_distance.x + m_distance.y*m_distance.y);

	return length;
}

float GameEntity::findPoint(float value, float min, float max, float paddlePoint)
{
	if (value < min)
		return min;
	else
		if (value > max)
			return max;
		else
			return value;

}