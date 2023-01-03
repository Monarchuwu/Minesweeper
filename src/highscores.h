#pragma once

#include "include.h"

void setHighScores(int newHighScores) {
	ofstream fo("highscores.txt", ios::trunc);
	fo << newHighScores;
}
int getOldHighScores() {
	int oldHighScore = 9999;
	ifstream fi("highscores.txt");
	if (!fi.fail()) fi >> oldHighScore;
	return oldHighScore;
}
