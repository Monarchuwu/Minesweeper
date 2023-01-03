#pragma once

#include "include.h"

clock_t start, now;
int timeBonus;
void setTime(int Bonus = 0) {
	timeBonus = Bonus;
	start	  = clock();
}
int getTime() {
	now = clock();
	return double(now - start) / CLOCKS_PER_SEC + timeBonus;
}