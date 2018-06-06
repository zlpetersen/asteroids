#pragma once
#include "SFML/Graphics.hpp"
class Asteroid : public sf::Sprite
{
private:
	int size;
	double rot, rotvel;
	double xvel, yvel;
	sf::Texture* t;
public:
	Asteroid();
	Asteroid(int s, double rot, double vel, double rotvel, double x, double y);
	~Asteroid();
	void update(sf::RenderWindow& w);
	int getSize() { return size; }
	double getRot() { return rot; }
};

