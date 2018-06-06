#pragma once
#include "SFML/Graphics.hpp"
#include "Bullet.h"
#include <vector>
class Ship : public sf::Sprite
{
private:
	double speed{ .3 };
	double friction{ .996 };
	double xvel{};
	double yvel{};
	double rot{};
	bool invincible;
	int invcnt;
	static int lives;
	int w, h;
	std::vector<Bullet*> bullets;
	int bClock;
	sf::Texture* t;
	sf::Texture* ti;
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) const {
		target.draw(*this);
	}
public:
	Ship();
	Ship(int wid, int ht);
	~Ship();
	void rotate(double amt) { rot += amt; }
	void update(sf::RenderWindow& w);
	std::vector<Bullet*>& getBullets() { return bullets; }
	void delBullet(int f);
	int getLives() { return lives; }
	bool isInvincible() { return invincible; }
	void setInvincible(bool i) { invincible = i; invcnt = 0; setTexture(*t); }
	void setLives(int l) { lives = l; }
	sf::FloatRect getBounds() { return sf::FloatRect(getGlobalBounds().left + 5, getGlobalBounds().top + 5, getGlobalBounds().width - 10, getGlobalBounds().height - 10); }
};

