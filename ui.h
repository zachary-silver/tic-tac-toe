#ifndef UI_H
#define UI_H

#include "tictactoe.h"

void printBoard(Game *game);
void printScore(Game *game);
int getContinue(void);
int getBoardSize(void);
Player getPlayer(void);
Play getPlay(Game *game);

#endif
