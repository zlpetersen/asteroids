#include "Bullet.h"

#include <iostream>

Bullet::Bullet(double rotation, sf::Vector2f pos)
{
	rot = rotation; // assign instance rotation

	// create bullet texture //
	t = new sf::Texture;
	t->loadFromFile("Bullet.png");
	t->setSmooth(true);

	// assign bullet texture and set origin, position, rotation //
	setTexture(*t);
	setOrigin(t->getSize().x / 2, t->getSize().y / 2);
	setPosition(pos);
	setRotation(rot);

	// set velocity based on rotation and speed //
	xvel = cos((rot * 3.14) / 180) * speed;
	yvel = sin((rot * 3.14) / 180) * speed;

	timer = 0; // start deletion timer at 0
}


Bullet::~Bullet()
{
}

void Bullet::update(sf::RenderWindow& w)
{
	timer++; // add to deletion timer

	// move sprite
	move(xvel, yvel);

	// wrap around edges of screen //
	if (getPosition().x > w.getSize().x)
		setPosition(sf::Vector2f(-getTextureRect().width, getPosition().y));
	if (getPosition().x < -getTextureRect().width)
		setPosition(sf::Vector2f(w.getSize().x, getPosition().y));
	if (getPosition().y > w.getSize().y)
		setPosition(sf::Vector2f(getPosition().x, -getTextureRect().height));
	if (getPosition().y < -getTextureRect().height)
		setPosition(sf::Vector2f(getPosition().x, w.getSize().y));
}
