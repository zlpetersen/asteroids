#pragma once
#include "SFML/Graphics.hpp"
class Bullet : public sf::Sprite
{
private:
	double rot;
	double speed{ 10 };
	double xvel, yvel;
	int timer;
	sf::Texture* t;
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) const {
		target.draw(*this);
	}
public:
	Bullet(double rotation, sf::Vector2f pos);
	~Bullet();
	void update(sf::RenderWindow& w);
	int getTimeOut() { return timer; }
};

