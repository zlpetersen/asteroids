#pragma once
#include <string>
class Score
{
private:
	std::string name;
	int score;
public:
	Score(std::string n, int sc) : name(n), score(sc) { }
	~Score();
	const std::string getName() { return name; }
	const int getScore() { return score; }
};

