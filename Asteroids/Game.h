#pragma once
#include "SFML/Graphics.hpp"
#include "Ship.h"
#include "Asteroid.h"
#include "Score.h"
#include <vector>
#include <fstream>
#include <string>
class Game
{
private:
	sf::RenderWindow* window;
	Ship* player;
	std::vector<Asteroid*> asteroids;
	sf::Font font;
	sf::Text* lives;
	sf::Text* scoreText;
	std::fstream scoreFile;
	std::vector<Score> scores;
	bool restart;
	int score;
	int level;
public:
	Game(sf::RenderWindow* w);
	~Game();
	void gameLoop();
	void gameOver();
	void loadScores();
	bool compareScore(Score& a, Score& b);
	bool doRestart() { return !restart; }
	void saveInfo(std::string n, int sc);
	std::vector<std::string> split(std::string s, std::string delimiter);
};

