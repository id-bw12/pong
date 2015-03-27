#include "ShapeHeader.h"
#include <iostream>

ShapeEntity::ShapeEntity(sf::RectangleShape *a, sf::CircleShape *b, std::string shapeName) : m_paddle(a), m_ball(b), m_shapeName(shapeName)
{
}

ShapeEntity::~ShapeEntity()
{
}

void ShapeEntity::setupShape()
{
	if (m_ball)
	{
		m_ball->setFillColor(m_shapeColor);
		m_ball->setPosition(m_position);
		m_ball->setRadius(m_radius);
		m_ball->setOrigin(sf::Vector2f(m_radius, m_radius));
	}
	else
	{
		m_paddle->setPosition(m_position);
		m_paddle->setSize(m_size);
		m_paddle->setOutlineColor(m_borderColor);
		m_paddle->setFillColor(m_shapeColor);
		m_paddle->setOutlineThickness(2);
		m_paddle->setOrigin(m_size.x / 2, m_size.y / 2);
	}
}

void ShapeEntity::setPosition(sf::Vector2f position)
{
	if (m_ball)
		m_ball->setPosition(position);
	else
		m_paddle->setPosition(position);
	m_position = position;
		
}

void ShapeEntity::moveShape(sf::Vector2f move)
{
	if (m_ball)
	{
		m_ball->move(move);
		m_position = m_ball->getPosition();
	}
	else
		if (m_paddle)
		{
			m_paddle->move(move);
			m_position = m_paddle->getPosition();
		}
}