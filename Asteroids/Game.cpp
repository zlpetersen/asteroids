#include "Game.h"
#include <random>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>


Game::Game(sf::RenderWindow* w)
{
	window = w;
	window->setVerticalSyncEnabled(true);
	player = new Ship(window->getSize().x, window->getSize().y); // create player
	player->setLives(3);

	// create random asteroid field
	for (int i = 0; i < 4; i++)
		asteroids.push_back(new Asteroid(4, (double)(rand() % 360), 1 * ((double)rand() / (double)RAND_MAX) + 1, .1 * ((double)rand() / (double)RAND_MAX) + .1,
			(double)(rand() % window->getSize().x), (double)(rand() % window->getSize().y)));

	// initialize score
	score = 0;

	// initialize font and lives text //
	font.loadFromFile("joystix monospace.ttf");
	lives = new sf::Text("LIVES\t" + player->getLives(), font, 20);
	scoreText = new sf::Text("SCORE\t" + score, font, 20);
	lives->setPosition(10, 0);
	scoreText->setPosition(10, 10 + lives->getLocalBounds().height);

	// initialize level
	level = 0;

	// initialize restart
	restart = true;

	// load and sort scores
	loadScores();

	// start game loop
	gameLoop();
}


Game::~Game()
{
	//delete window;
	delete player;
	for (Asteroid* a : asteroids)
		delete a;
	delete lives;
	delete scoreText;
}

void Game::gameLoop()
{
	while (window->isOpen()) {

		// Test if X or Esc is pressed //
		sf::Event e;
		window->pollEvent(e);
		if (e.type == sf::Event::Closed)
			window->close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window->close();

		// Update player
		player->update(*window);

		// Drawing //
		window->clear();

		// if screen is empty refill it
		if (asteroids.size() == 0) {
			level++;
			for (int i = 0; i < 4 + level + level; i++)
				asteroids.push_back(new Asteroid(4, (double)(rand() % 360), 1 * ((double)rand() / (double)RAND_MAX) + 1, .1 * ((double)rand() / (double)RAND_MAX) + .1,
				(double)(rand() % window->getSize().x), (double)(rand() % window->getSize().y)));
			player->setInvincible(true);
		}


		// ------------- Collision --------------- //

		// Bullets and Asteroids //
		for (int f = 0; f < player->getBullets().size(); f++) {
			Bullet* b = player->getBullets()[f];
			b->update(*window); // update bullets
			window->draw(*b); // draw bullets

			// Collision //
			for (int i = 0; i < asteroids.size(); i++) { // every asteroid
				Asteroid* a = asteroids[i];

				// if bullet and asteroid intersect
				if (b->getGlobalBounds().intersects(a->getGlobalBounds())) {
					// add to score
					int l = a->getSize();
					if (l == 4) score += 10;
					else if (l == 3) score += 20;
					else if (l == 2) score += 50;
					else if (l == 1) score += 100;
					// if asteroid is big enough to split
					if (a->getSize() > 1) {
						// get asteroid size and position
						int s = a->getSize();
						int ax = a->getPosition().x;
						int ay = a->getPosition().y;
						delete asteroids[i]; // delete asteroid
						asteroids.erase(asteroids.begin() + i); // remove pointer from asteroids array

						// create new split asteroids //
						asteroids.push_back(new Asteroid(s - 1, a->getRot() + 90,
							1 * ((double)rand() / (double)RAND_MAX) + 1, .1 * ((double)rand() / (double)RAND_MAX) + .1, ax, ay));
						asteroids.push_back(new Asteroid(s - 1, a->getRot() - 90,
							1 * ((double)rand() / (double)RAND_MAX) + 1, .1 * ((double)rand() / (double)RAND_MAX) + .1, ax, ay));
					}
					// if asteroid is too small
					else {
						delete asteroids[i]; // delete asteroid
						asteroids.erase(asteroids.begin() + i); // remove pointer from asteroids array
					}
					player->delBullet(f); // delete bullet
				}
			}
		}

		// Ship and Asteroids //
		for (int i = 0; i < asteroids.size(); i++) { // every asteroid
			asteroids[i]->update(*window); // update asteroids
			window->draw(*asteroids[i]); // draw asteroids

			// if asteroid and ship intersect
			if (asteroids[i]->getGlobalBounds().intersects(player->getBounds())) {
				// if player isn't invincible
				if (!(player->isInvincible())) {
					// if player is out of lives call game over //
					if (player->getLives() < 2) {
						gameOver();
						goto endLoop;
					}
					// else replace player with a new one at the center of the screen //
					else {
						delete player;
						player = new Ship(window->getSize().x, window->getSize().y);
					}
				}
			}
		}

		// draw player
		window->draw(*player);

		// update lives and score indicators
		lives->setString("LIVES\t" + std::to_string(player->getLives()));
		scoreText->setString("SCORE\t" + std::to_string(score));

		// draw lives and score indicators
		window->draw(*lives);
		window->draw(*scoreText);

		// draw everything to the screen
		window->display();
	}
	endLoop:;
}

void Game::gameOver()
{
	// set up text //
	sf::Text goText("GAME OVER", font, 60); // game over text
	sf::Text agText("TAB TO INSERT COIN", font, 30); // start over text
	sf::Text scText("SCORE:" + std::to_string(score), font, 20); // score text
	sf::Text highText("HIGHSCORES", font, 20); // highscore
	std::string nameEnter = "";
	sf::Text enterText("ENTER NAME: " + nameEnter, font, 30); // enter name text
	int c = 0;
	std::vector<std::string> names;
	for (Score s : scores)
		if (c < 5) {
			names.push_back(s.getName());
			c++;
		}
	sf::Text namesText("", font, 20);
	for (std::string s : names)
		namesText.setString(namesText.getString() + "\n" + s);
	std::cout << std::string(namesText.getString());
	std::vector<int> scoresL;
	c = 0;
	for (Score s : scores)
		if (c < 5) {
			scoresL.push_back(s.getScore());
			c++;
		}
	sf::Text scoresText("", font, 20);
	for (int s : scoresL)
		scoresText.setString(scoresText.getString() + "\n" + std::to_string(s));
	std::cout << std::string(scoresText.getString());

	// position text
	goText.setOrigin(goText.getLocalBounds().width / 2, goText.getLocalBounds().height / 3);
	agText.setOrigin(agText.getLocalBounds().width / 2, agText.getLocalBounds().height / 2);
	scText.setOrigin(scText.getLocalBounds().width / 2, scText.getLocalBounds().height / 2);
	highText.setOrigin(highText.getLocalBounds().width / 2, highText.getLocalBounds().height / 2);
	namesText.setOrigin(namesText.getLocalBounds().width / 2, namesText.getLocalBounds().height / 2);
	scoresText.setOrigin(scoresText.getLocalBounds().width / 2, scoresText.getLocalBounds().height / 2);
	enterText.setOrigin(enterText.getLocalBounds().width / 2, enterText.getLocalBounds().height / 2);
	goText.setPosition(window->getSize().x / 2, window->getSize().y / 8);
	agText.setPosition(window->getSize().x / 2, window->getSize().y / 8 * 6);
	scText.setPosition(window->getSize().x / 2, window->getSize().y / 8 * 2);
	highText.setPosition(window->getSize().x / 2, window->getSize().y / 2 - 100);
	enterText.setPosition(window->getSize().x / 2, window->getSize().y / 3 * 2);
	namesText.setPosition(window->getSize().x * 7 / 16, window->getSize().y / 2 - 30);
	scoresText.setPosition(window->getSize().x * 9 / 16, window->getSize().y / 2 - 30);
	bool go = true; // while gameover screen is up
	while (go) {

		// Test if X or Esc is pressed //
		sf::Event e;
		while (window->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window->close();
				restart = true;
				break;
			}
			if (e.type == sf::Event::TextEntered) {
				if (e.text.unicode > 96 && e.text.unicode < 123) {
					if (nameEnter.length() < 3) {
						nameEnter += static_cast<char>(e.text.unicode);
						enterText.setString("ENTER NAME: " + nameEnter);
						std::cout << nameEnter;
					}
				}
			}
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::BackSpace)
				if (nameEnter.length() > 0) {
					nameEnter.erase(nameEnter.size() - 1, 1);
					enterText.setString("ENTER NAME: " + nameEnter);
				}
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
				if (nameEnter.length() == 3) {
					saveInfo(nameEnter, score);
					enterText.setString("SAVED");
					enterText.setOrigin(enterText.getLocalBounds().width / 2, enterText.getLocalBounds().height / 2);
					loadScores();
					c = 0;
					names.clear();
					for (Score s : scores)
						if (c < 5) {
							names.push_back(s.getName());
							c++;
						}
					for (std::string s : names)
						namesText.setString(namesText.getString() + "\n" + s);
					std::cout << std::string(namesText.getString());
					scoresL.clear();
					c = 0;
					for (Score s : scores)
						if (c < 5) {
							scoresL.push_back(s.getScore());
							c++;
						}
					for (int s : scoresL)
						scoresText.setString(scoresText.getString() + "\n" + std::to_string(s));
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			break;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
			restart = false;
			break;
		}

		// Drawing //
		window->clear();

		window->draw(goText);
		window->draw(agText);
		window->draw(scText);
		window->draw(namesText);
		window->draw(scoresText);
		window->draw(highText);
		window->draw(enterText);

		window->display();
	
	
	}
}

void Game::loadScores()
{
	scores.clear(); // clear scores vector
	scoreFile.open("scores.txt"); // open score file
	std::stringstream buffer;
	buffer << scoreFile.rdbuf();
	for (std::string line : split(buffer.str(), "/")) { // iterate through lines
		std::vector<std::string> l = split(line, ":"); // split into name, score, acc
		try {
			scores.push_back(Score(l[0], std::stoi(l[1]))); // add to score vector
		}
		catch (std::exception e) {}
	}
	std::sort(scores.begin(), scores.end(), [this](auto a, auto b) { return compareScore(a, b); }); // sort by score
	scoreFile.close();
}

bool Game::compareScore(Score& a, Score& b)
{
	return a.getScore() > b.getScore(); // compare two score objects
}

void Game::saveInfo(std::string n, int sc)
{
	scoreFile.open("scores.txt");
	scoreFile << n << ":" << std::to_string(sc) << "/";
	scoreFile.close();
}

std::vector<std::string> Game::split(std::string s, std::string delimiter)
{
	std::vector<std::string> l;
	size_t pos = 0;
	std::string token;
	for (int i = 0; i < 3; i++) {
		pos = s.find(delimiter);
		token = s.substr(0, pos);
		l.push_back(token);
		s.erase(0, pos + 1);
	}
	return l;
}

int main() {
	sf::RenderWindow* w = new sf::RenderWindow(sf::VideoMode(800, 600), "Asteroids"); // create window
	Game* g = new Game(w);
	while (g->doRestart()) {
		delete g;
		g = new Game(w);
	}
	delete g;
	delete w;
}
