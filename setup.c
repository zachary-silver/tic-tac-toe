#include "setup.h"

void initializeGame(Game *game, int size)
{
    game->rows = size;
    game->columns = size;
    game->player1Score = 0;
    game->player2Score = 0;
}
