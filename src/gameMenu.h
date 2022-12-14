#define BEGINNER	 0
#define INTERMEDIATE 1
#define ADVANCED	 2

// A Function to choose the difficulty level of the game
void chooseDifficultyLevel() {
	int level;
	printf("Enter the Difficulty Level\n");
	printf("Press 0 for BEGINNER (9 * 9 Cells and 10 Mines)\n");
	printf("Press 1 for INTERMEDIATE (16 * 16 Cells and 40 Mines)\n");
	printf("Press 2 for ADVANCED (24 * 24 Cells and 99 Mines)\n");
	scanf("%d", &level);

	if (level == BEGINNER) {
		SIDE  = 9;
		MINES = 10;
	}

	if (level == INTERMEDIATE) {
		SIDE  = 16;
		MINES = 40;
	}

	if (level == ADVANCED) {
		SIDE  = 24;
		MINES = 99;
	}
}
