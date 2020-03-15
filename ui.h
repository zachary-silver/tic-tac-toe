#ifndef UI_H
#define UI_H

#include "tictactoe.h"

void printBoard(Game *game);
void printScore(Game *game);
int promptToContinue(void);
int getBoardSize(void);

#endif
