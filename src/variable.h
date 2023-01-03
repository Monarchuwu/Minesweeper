#pragma once

#define TIME_SLEEP 200
const int di[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
const int dj[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
enum COLOR {
	C_BLACK,
	C_BLUE,
	C_GREEN,
	C_CYAN,
	C_RED,
	C_PINK,
	C_YELLOW,
	C_WHILE,
	C_GREY,
	C_BRIGHT_BLUE,
	C_BRIGHT_GREEN,
	C_BRIGHT_CYAN,
	C_BRIGHT_RED,
	C_BRIGHT_PINK,
	C_BRIGHT_YELLOW,
	C_BRIGHT_WHILE
};
#define GetColor(BACKGROUND, TEXT) ((BACKGROUND) << 4 | (TEXT))
#define DEFAULT_COLOR			   GetColor(C_BLACK, C_BRIGHT_WHILE)

int height;
int width;
int numberOfMines;
int timeNow;
bool isBomb[32][32];
bool isOpened[32][32];
bool isFlagged[32][32];
int adjBomb[32][32];

// border:	GetColor(C_GREEN, C_BLACK)
// *: 		GetColor(C_BRIGHT_WHILE, C_BLACK)
// F: 		GetColor(C_BRIGHT_CYAN, C_BLACK)
// num: 	GetColor(C_BRIGHT_WHILE, num)
//		    use SetNumInBoard()
// B:		GetColor(C_BRIGHT_RED, C_BLACK)