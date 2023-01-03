// chia file để dễ nhìn hơn
#include "src/include.h"
#include "src/timer.h"
#include "src/highscores.h"
#include "src/savegame.h"

mt19937_64 rd(chrono::steady_clock::now().time_since_epoch().count());
int rand(int l, int r) {
	return uniform_int_distribution<int>(l, r)(rd);
}

// goto (row, col) cell
void gotoxy(int row, int col) {
	COORD coord;
	coord.X = col;
	coord.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// set the character at (row, col) cell as ch
void SetChar(int row, int col, char ch) {
	gotoxy(row, col);
	cout << ch;
}
// set the character        at (row, col) cell as ch
// set the background color at (row, col) cell as Color value
void SetCharWithColor(int row, int col, char ch, int Color) {
	gotoxy(row, col);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
	cout << ch;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
}
// set the character at (row, col) cell as
// - num,              if '1' <= num <= '8'
// - else SPACE (' '), for num = '0' and '*' (bomb)
// set the background color at (row, col) cell as BRIGHT_WHITE
void SetNumInBoard(int row, int col, char num) {
	SetCharWithColor(row, col, ('1' <= num && num <= '8' ? num : ' '), GetColor(C_BRIGHT_WHILE, num));
}
// check if 'key' button is being pressed
bool isPress(char key) {
	return GetKeyState(key) & 0x8000;
}
// return true if open the bomb
bool dfsOpen(int row, int col) {
	if (row < 1 || height < row || col < 1 || width < col) return false;
	isOpened[row][col] = true;
	if (isBomb[row][col]) return true;
	SetNumInBoard(row, col, char(adjBomb[row][col] + '0'));
	if (adjBomb[row][col] != 0) return false;

	for (int k = 0; k < 8; ++k) {
		int nRow = row + di[k];
		int nCol = col + dj[k];
		if (!isOpened[nRow][nCol]) {
			if (dfsOpen(nRow, nCol)) return true;
		}
	}
	return false;
}
int countIsOpen() {
	int cnt = 0;
	for (int i = 1; i <= height; ++i) {
		for (int j = 1; j <= width; ++j) {
			cnt += isOpened[i][j];
		}
	}
	return cnt;
}
void GameScreen(bool LoadGame);
void isWIN() {
	int oldHighScores = getOldHighScores();
	if (timeNow < oldHighScores) setHighScores(timeNow);

	gotoxy(3, 36);
	cout << "YOU WIN !!! Press R to play again ";
	Sleep(TIME_SLEEP);
	while (!isPress('R')) {}
	GameScreen(false);
}
void isLOSE() {
	gotoxy(3, 36);
	cout << "YOU LOSE !!! Press R to play again ";

	for (int i = 1; i <= height; ++i) {
		for (int j = 1; j <= width; ++j) {
			if (isBomb[i][j]) {
				SetCharWithColor(i, j, 'B', GetColor(C_BRIGHT_RED, C_BLACK));
			}
		}
	}

	Sleep(TIME_SLEEP);
	while (!isPress('R')) {}
	GameScreen(false);
}
void GameScreen(bool LoadGame) {
	system("cls");
	int bombLeft;
	int curRow, curCol;

	/// Setup Game Screen
	if (!LoadGame) {
		/// Setup new Game

		bombLeft = numberOfMines;
		// game in range [1 ... height][1 ... width]
		for (int i = 1; i <= height; ++i) {
			for (int j = 1; j <= width; ++j) {
				SetCharWithColor(i, j, '*', GetColor(C_BRIGHT_WHILE, C_BLACK));
			}
		}
		// border
		for (int i = 0; i <= height + 1; ++i) {
			// [i, 0]
			SetCharWithColor(i, 0, ' ', GetColor(C_GREEN, C_BLACK));
			// [i, width + 1]
			SetCharWithColor(i, width + 1, ' ', GetColor(C_GREEN, C_BLACK));
		}
		for (int j = 1; j <= width; ++j) {
			// [0, j]
			SetCharWithColor(0, j, ' ', GetColor(C_GREEN, C_BLACK));
			// [height + 1, j]
			SetCharWithColor(height + 1, j, ' ', GetColor(C_GREEN, C_BLACK));
		}

		/// Setup Mines
		// Reset isBomb array
		for (int i = 0; i <= height + 1; ++i) {
			for (int j = 0; j <= width + 1; ++j) {
				isBomb[i][j]	= false;
				isOpened[i][j]	= false;
				isFlagged[i][j] = false;
			}
		}
		// Setup Bomb
		vector<pair<int, int>> IndexOfTable;
		for (int i = 1; i <= height; ++i) {
			for (int j = 1; j <= width; ++j) {
				IndexOfTable.emplace_back(i, j);
			}
		}
		shuffle(IndexOfTable.begin(), IndexOfTable.end(), default_random_engine(rd()));
		for (int i = 0; i < IndexOfTable.size() && i < numberOfMines; ++i) {
			int row			 = IndexOfTable[i].first;
			int col			 = IndexOfTable[i].second;
			isBomb[row][col] = true;
		}
		// Calculate adjBomb array
		for (int i = 1; i <= height; ++i) {
			for (int j = 1; j <= width; ++j) {
				adjBomb[i][j] = 0;
				for (int k = 0; k < 8; ++k) {
					adjBomb[i][j] += isBomb[i + di[k]][j + dj[k]];
				}
			}
		}

		timeNow = -1; // the game hasn't started yet
		curRow = 1, curCol = 1;
	}
	else {
		/// Load Old Game
		ifstream fi("sav.txt");
		fi >> height >> width >> numberOfMines >> timeNow;
		fi >> bombLeft >> curRow >> curCol;
		if (timeNow != -1) {
			setTime(timeNow);
		}

		for (int i = 0; i <= height + 1; ++i)
			for (int j = 0; j <= width + 1; ++j)
				fi >> isBomb[i][j];

		for (int i = 0; i <= height + 1; ++i)
			for (int j = 0; j <= width + 1; ++j)
				fi >> isOpened[i][j];

		for (int i = 0; i <= height + 1; ++i)
			for (int j = 0; j <= width + 1; ++j)
				fi >> isFlagged[i][j];

		for (int i = 0; i <= height + 1; ++i)
			for (int j = 0; j <= width + 1; ++j)
				fi >> adjBomb[i][j];

		/// Load the Game Screen
		// game in range [1 ... height][1 ... width]
		for (int i = 1; i <= height; ++i) {
			for (int j = 1; j <= width; ++j) {
				if (isOpened[i][j]) {
					SetNumInBoard(i, j, char(adjBomb[i][j] + '0'));
				}
				else if (isFlagged[i][j]) {
					SetCharWithColor(i, j, 'F', GetColor(C_BRIGHT_CYAN, C_BLACK));
				}
				else {
					SetCharWithColor(i, j, '*', GetColor(C_BRIGHT_WHILE, C_BLACK));
				}
			}
			fi.close();
		}
		// border
		for (int i = 0; i <= height + 1; ++i) {
			// [i, 0]
			SetCharWithColor(i, 0, ' ', GetColor(C_GREEN, C_BLACK));
			// [i, width + 1]
			SetCharWithColor(i, width + 1, ' ', GetColor(C_GREEN, C_BLACK));
		}
		for (int j = 1; j <= width; ++j) {
			// [0, j]
			SetCharWithColor(0, j, ' ', GetColor(C_GREEN, C_BLACK));
			// [height + 1, j]
			SetCharWithColor(height + 1, j, ' ', GetColor(C_GREEN, C_BLACK));
		}
	}

	// Game Instructions
	gotoxy(0, 36);
	cout << "Bomb(s) left: " << bombLeft << "    ";
	gotoxy(0, 56);
	cout << "Time: " << max(0, timeNow) << "s    ";
	gotoxy(1, 33);
	cout << "-------------------------------------------";
	gotoxy(5, 36);
	cout << "HIGH SCORE: " << getOldHighScores();
	gotoxy(7, 33);
	cout << "-------------------------------------------";
	gotoxy(8, 33);
	cout << " Game instructions:";
	gotoxy(9, 36);
	cout << "- Press arrow keys to move";
	gotoxy(10, 36);
	cout << "- Press SPACE to open a square";
	gotoxy(11, 36 + 11);
	cout << "OR to reveal adjacent squares";
	gotoxy(12, 36);
	cout << "- Press F to flag";
	gotoxy(13, 36);
	cout << "- Press G to move to any cell in the broad";
	gotoxy(14, 38);
	cout << "+ Row:    ";
	gotoxy(15, 38);
	cout << "+ Column: ";
	gotoxy(16, 36);
	cout << "- Press S to save the game";
	gotoxy(17, 36);
	cout << "- Press Q to exit";

	/// Start Game
	gotoxy(curRow, curCol);
	while (true) {
		if (timeNow != -1) {
			int t = getTime();
			if (timeNow < t) {
				timeNow = t;
				gotoxy(0, 62);
				cout << timeNow << "s    ";
				gotoxy(curRow, curCol);
			}
		}

		if (isPress(VK_UP)) {
			curRow = max(1, curRow - 1);
			gotoxy(curRow, curCol);
			Sleep(TIME_SLEEP);
			continue;
		}
		if (isPress(VK_DOWN)) {
			curRow = min(height, curRow + 1);
			gotoxy(curRow, curCol);
			Sleep(TIME_SLEEP);
			continue;
		}
		if (isPress(VK_LEFT)) {
			curCol = max(1, curCol - 1);
			gotoxy(curRow, curCol);
			Sleep(TIME_SLEEP);
			continue;
		}
		if (isPress(VK_RIGHT)) {
			curCol = min(width, curCol + 1);
			gotoxy(curRow, curCol);
			Sleep(TIME_SLEEP);
			continue;
		}
		if (isPress(VK_SPACE)) {
			if (timeNow == -1) {
				setTime();
				timeNow = 0;
			}

			if (isFlagged[curRow][curCol]) continue;

			if (isOpened[curRow][curCol]) {
				int cnt_flag = 0;
				for (int k = 0; k < 8; ++k) {
					int nRow = curRow + di[k];
					int nCol = curCol + dj[k];
					cnt_flag += isFlagged[nRow][nCol];
				}
				if (cnt_flag == adjBomb[curRow][curCol]) {
					for (int k = 0; k < 8; ++k) {
						int nRow = curRow + di[k];
						int nCol = curCol + dj[k];
						if (!isFlagged[nRow][nCol] || isOpened[nRow][nCol]) {
							if (dfsOpen(nRow, nCol)) isLOSE();
						}
					}
				}
				gotoxy(curRow, curCol);
				Sleep(TIME_SLEEP);
				if (countIsOpen() == height * width - numberOfMines) {
					isWIN();
					return;
				}
				continue;
			}

			isOpened[curRow][curCol] = true;
			if (isBomb[curRow][curCol]) {
				Sleep(TIME_SLEEP);
				isLOSE();
				return;
			}

			if (dfsOpen(curRow, curCol)) isLOSE();
			gotoxy(curRow, curCol);
			if (countIsOpen() == height * width - numberOfMines) {
				isWIN();
				return;
			}
			continue;
		}
		if (isPress('F')) {
			if (isOpened[curRow][curCol]) continue;

			if (isFlagged[curRow][curCol]) {
				SetCharWithColor(curRow, curCol, '*', GetColor(C_BRIGHT_WHILE, C_BLACK));
				bombLeft += 1;
				isFlagged[curRow][curCol] = false;
			}
			else {
				SetCharWithColor(curRow, curCol, 'F', GetColor(C_BRIGHT_CYAN, C_BLACK));
				bombLeft -= 1;
				isFlagged[curRow][curCol] = true;
			}

			gotoxy(0, 50);
			cout << bombLeft << "    ";

			gotoxy(curRow, curCol);
			Sleep(TIME_SLEEP);
			continue;
		}
		if (isPress('G')) {
			curRow = 0;
			while (curRow < 1 || height < curRow) {
				gotoxy(14, 48);
				cout << "    ";
				gotoxy(14, 48);
				cin >> curRow;
			}

			curCol = 0;
			while (curCol < 1 || width < curCol) {
				gotoxy(15, 48);
				cout << "    ";
				gotoxy(15, 48);
				cin >> curCol;
			}

			gotoxy(curRow, curCol);
			Sleep(TIME_SLEEP);
			continue;
		}
		if (isPress('S')) {
			saveGame(bombLeft, curRow, curCol);
			continue;
		}
		if (isPress('Q')) {
			exit(0);
		}
	}
}
void MenuScreen() {
	system("cls");
	cout << "* * * * * MINESWEEPER * * * * *\n"
			"Input height (1-30):\n"
			"Input width  (1-30):\n"
			"Input mines (1-600):\n"
			"-------------------------------";

	gotoxy(1, 21);
	cin >> height;
	gotoxy(2, 21);
	cin >> width;
	gotoxy(3, 21);
	cin >> numberOfMines;

	GameScreen(false);
}
void MainScreen(bool cantLoad = false) {
	system("cls");
	cout << "* * * * * MINESWEEPER * * * * *\n"
			"New Game (N) or Load Game(L) ? ";
	if (cantLoad) {
		cout << "(No games saved) ";
	}

	char c;
	cin >> c;
	if (c == 'N') {
		MenuScreen();
	}
	else if (cantLoad == false && c == 'L') {
		ifstream fi("sav.txt");
		if (fi.fail()) {
			MainScreen(true);
		}
		else {
			GameScreen(true);
		}
	}
	else {
		MainScreen(cantLoad);
	}
}

void test() {
	height		  = 9;
	width		  = 9;
	numberOfMines = 10;
	GameScreen(false);
}
int main() {
	// test();
	// return 0;
	MainScreen();
}
/**  /\_/\
 *  (= ._.)
 *  / >0  \>1
 */
