// thay vì getchar() thì while not press SPACE
// chia file để dễ nhìn hơn
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
typedef long long ll;

#include <chrono>
#include <random>
mt19937_64 rd(chrono::steady_clock::now().time_since_epoch().count());
ll rand(ll l, ll r) {
	return uniform_int_distribution<ll>(l, r)(rd);
}

#define TIME_SLEEP 200
const int di[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
const int dj[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

int height;
int width;
int numberOfMines;
bool isBomb[32][32];
bool isOpened[32][32];
bool isFlagged[32][32];
int adjBomb[32][32];

void gotoxy(int row, int col) {
	COORD coord;
	coord.X = col;
	coord.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void SetChar(int row, int col, char ch) {
	gotoxy(row, col);
	cout << ch;
}
bool isPress(char key) {
	return GetKeyState(key) & 0x8000;
}
void dfsOpen(int row, int col) {
	if (row < 1 || height < row || col < 1 || width < col) return;
	isOpened[row][col] = true;
	SetChar(row, col, char(adjBomb[row][col] + 48));
	if (adjBomb[row][col] != 0) return;

	for (int k = 0; k < 8; ++k) {
		int nRow = row + di[k];
		int nCol = col + dj[k];
		if (!isOpened[nRow][nCol]) {
			dfsOpen(nRow, nCol);
		}
	}
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
void GameScreen() {
	system("cls");
	/// Setup Game Screen
	int bombLeft = numberOfMines;
	// game in range [1 ... height][1 ... width]
	for (int i = 1; i <= height; ++i) {
		for (int j = 1; j <= width; ++j) {
			SetChar(i, j, '*');
		}
	}
	// Game Instructions
	gotoxy(0, 36);
	cout << "Bomb(s) left: " << bombLeft << "    ";
	gotoxy(1, 33);
	cout << "-------------------------------------------";
	gotoxy(6, 33);
	cout << "-------------------------------------------";
	gotoxy(7, 33);
	cout << " Game instructions:";
	gotoxy(8, 36);
	cout << "- Press arrow keys to move";
	gotoxy(9, 36);
	cout << "- Press SPACE to open a square";
	gotoxy(10, 36 + 11);
	cout << "OR to reveal adjacent squares";
	gotoxy(11, 36);
	cout << "- Press F to flag";

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

	/// Start Game
	int curRow = 1, curCol = 1;
	bool isWin = false;
	gotoxy(curRow, curCol);
	while (true) {
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
						if (!isFlagged[nRow][nCol] || isOpened[nRow][nCol])
							dfsOpen(nRow, nCol);
					}
				}
				gotoxy(curRow, curCol);
				Sleep(TIME_SLEEP);
				if (countIsOpen() == height * width - numberOfMines) {
					gotoxy(3, 36);
					cout << "YOU WIN !!! Enter to play again ";
					Sleep(TIME_SLEEP);
					getchar();
					GameScreen();
					return;
				}
				continue;
			}

			isOpened[curRow][curCol] = true;
			if (isBomb[curRow][curCol]) {
				SetChar(curRow, curCol, 'B');
				Sleep(TIME_SLEEP);

				gotoxy(3, 36);
				cout << "YOU LOSE !!! Enter to play again ";
				Sleep(TIME_SLEEP);
				getchar();
				GameScreen();
				return;
			}

			dfsOpen(curRow, curCol);
			gotoxy(curRow, curCol);
			if (countIsOpen() == height * width - numberOfMines) {
				gotoxy(3, 36);
				cout << "YOU WIN !!! Enter to play again ";
				Sleep(TIME_SLEEP);
				getchar();
				GameScreen();
				return;
			}
			continue;
		}
		if (isPress('F')) {
			if (isOpened[curRow][curCol]) continue;

			if (isFlagged[curRow][curCol]) {
				SetChar(curRow, curCol, '*');
				bombLeft += 1;
				isFlagged[curRow][curCol] = false;
			}
			else {
				SetChar(curRow, curCol, 'F');
				bombLeft -= 1;
				isFlagged[curRow][curCol] = true;
			}

			gotoxy(0, 50);
			cout << bombLeft << "    ";

			gotoxy(curRow, curCol);
			Sleep(TIME_SLEEP);
			continue;
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

	GameScreen();
}
void MainScreen() {
	system("cls");
	cout << "* * * * * MINESWEEPER * * * * *\n"
			"New Game (N)? ";
	char c;
	cin >> c;
	if (c == 'N') {
		MenuScreen();
	}
	else {
		MainScreen();
	}
}

void test() {
	height		  = 9;
	width		  = 9;
	numberOfMines = 10;
	GameScreen();
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
