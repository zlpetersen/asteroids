#include "Asteroid.h"



Asteroid::Asteroid()
{
	Asteroid(2, 0, .2, .1, 0, 0); // call constructor with default values
}

Asteroid::Asteroid(int s, double rot, double vel, double rotvel, double x, double y)
{
	size = s; // set asteroid size

	// create asteroid texture //
	t = new sf::Texture();
	t->loadFromFile(std::to_string(s) + ".png");
	t->setSmooth(true);

	// assign texture and set origin, rotation, position //
	setTexture(*t);
	setOrigin(t->getSize().x / 2, t->getSize().y / 2);
	setRotation(rot);
	setPosition(x, y);

	// set rotation and rotation velocity //
	this->rot = rot;
	this->rotvel = rotvel;

	// set velocity based on rotation and speed //
	xvel = cos((rot * 3.14) / 180) * vel;
	yvel = sin((rot * 3.14) / 180) * vel;
}


Asteroid::~Asteroid()
{
}

void Asteroid::update(sf::RenderWindow & w)
{
	// move sprite and set rotation //
	move(xvel, yvel);
	setRotation(getRotation() + rotvel);

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
