#include "Ship.h"

#include <iostream>

int Ship::lives = 3;

Ship::Ship()
{

}

Ship::Ship(int wid, int ht)
{
	rot = 0; // set rotation

	// load main texture
	t = new sf::Texture();
	t->loadFromFile("Shipi.png");
	t->setSmooth(true);

	// load invincible texture
	ti = new sf::Texture();
	ti->loadFromFile("Shipr.png");
	ti->setSmooth(true);

	// assign texture and set position, origin, and rotation
	setTexture(*t);
	setPosition(sf::Vector2f(wid / 2, ht / 2));
	setOrigin(t->getSize().x / 2, t->getSize().y / 2);
	setRotation(rot);

	// set x and y vel
	xvel = 0;
	yvel = 0;

	// set bullet fire timer to 0
	bClock = 0;

	// make ship invincible and start countown until invincibility wears off
	invincible = true;
	invcnt = 0;
}


Ship::~Ship()
{
	delete t;
	delete ti;
	for (Bullet* b : bullets)
		delete b;
	lives--;
}

void Ship::update(sf::RenderWindow& w)
{
	bClock++; // add to bullet fire timer

	if (invincible) {
		invcnt++; // add to invincibility timer
		if (invcnt >= 100) { // if invincibility wore off
			invincible = false;
			setTexture(*ti); // assign main texture
		}
	}

	// accelerate if up is pressed //
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		xvel += cos((rot * 3.14) / 180) * speed;
		yvel += sin((rot * 3.14) / 180) * speed;
	}
	// decelerate if up is not pressed //
	else {
		xvel *= friction;
		yvel *= friction;
	}

	// rotate left and right //
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		rotate(-5);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		rotate(5);
	// fire if space is pressed and fire timer is high enough //
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (bClock >= 20) {
			bullets.push_back(new Bullet(rot, getPosition()));
			bClock = 0;
		}
	}
	// make firing immediately available when space is not pressed //
	else
		bClock = 300;

	// delete bullets if their timer is high enough //
	for (int i = 0; i < bullets.size(); i++)
		if (bullets[i]->getTimeOut() > 50) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}

	setRotation(rot); // set sprite rotation
	if (abs(rot) == 360 || abs(rot) == -360) rot = 0; // avoid overflow on constant rotation //

	// move the sprite //
	move(sf::Vector2f(xvel, yvel));

	// player movement wraps around the edges of the screen //
	if (getPosition().x > w.getSize().x)
		setPosition(sf::Vector2f(-getTextureRect().width, getPosition().y));
	if (getPosition().x < -getTextureRect().width)
		setPosition(sf::Vector2f(w.getSize().x, getPosition().y));
	if (getPosition().y > w.getSize().y)
		setPosition(sf::Vector2f(getPosition().x, -getTextureRect().height));
	if (getPosition().y < -getTextureRect().height)
		setPosition(sf::Vector2f(getPosition().x, w.getSize().y));
}

void Ship::delBullet(int f)
{
	// remove bullet and reset fire timer //
	delete bullets[f];
	bullets.erase(bullets.begin() + f);
	bClock = 300;
}
