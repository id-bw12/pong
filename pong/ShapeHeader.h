#ifndef SHAPEHEADER_H
#define SHAPEHEADER_H

#include <SFML\Graphics.hpp>
#include <string>

class ShapeEntity
{
public:
	ShapeEntity(sf::RectangleShape *a = nullptr, sf::CircleShape *b= nullptr, std::string shapeName = "");
	~ShapeEntity();
	sf::RectangleShape getPaddle(){ return *m_paddle; }
	sf::CircleShape getBall(){ return *m_ball; }
	sf::Vector2f getPosition(){ return m_position; }
	float getRadius(){ return m_radius; }
	float getWidth(){ return m_size.x; }
	float getHeight(){ return m_size.y; }
	std::string getName(){ return m_shapeName; }
	void setPosition(sf::Vector2f position);
	void setShapeColor(sf::Color color){ m_shapeColor = color; }
	void setBorderColor(sf::Color color){ m_borderColor = color; }
	void setSize(sf::Vector2f size){ m_size = size; }
	void moveShape(sf::Vector2f move);
	void setRadius(float radius){ m_radius = radius; };
	void setupShape();
	
	

private:
	sf::RectangleShape *m_paddle;
	sf::CircleShape *m_ball;
	sf::Vector2f m_position, m_size;
	sf::Color m_shapeColor, m_borderColor;
	
	float m_radius;
	std::string m_shapeName = "";
	
};

#endif