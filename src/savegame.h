#pragma once

#include "include.h"

// save the current state of the broad
void saveGame(int bombLeft, int curRow, int curCol) {
	ofstream fo("sav.txt", ios::trunc);
	fo << height << ' ' << width << ' ' << numberOfMines << ' ' << timeNow << '\n';
	fo << bombLeft << ' ' << curRow << ' ' << curCol << '\n';
	// save isBomb array
	for (int i = 0; i <= height + 1; ++i) {
		for (int j = 0; j <= width + 1; ++j) {
			fo << isBomb[i][j] << ' ';
		}
		fo << '\n';
	}
	// save isOpened array
	for (int i = 0; i <= height + 1; ++i) {
		for (int j = 0; j <= width + 1; ++j) {
			fo << isOpened[i][j] << ' ';
		}
		fo << '\n';
	}
	// save isFlagged array
	for (int i = 0; i <= height + 1; ++i) {
		for (int j = 0; j <= width + 1; ++j) {
			fo << isFlagged[i][j] << ' ';
		}
		fo << '\n';
	}
	// save adjBomb array
	for (int i = 0; i <= height + 1; ++i) {
		for (int j = 0; j <= width + 1; ++j) {
			fo << adjBomb[i][j] << ' ';
		}
		fo << '\n';
	}
}