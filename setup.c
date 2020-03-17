#include <stdlib.h>

#include "setup.h"
#include "memory.h"

void initializeGame(Game *game, int rows, int columns)
{
    int i;

    game->rows = rows;
    game->columns = columns;
    game->player1Score = 0;
    game->player2Score = 0;
    game->board = getMemory(sizeof(Player*) * game->columns, FALSE);
    for (i = 0; i < game->columns; i++) {
        game->board[i] = getMemory(sizeof(Player) * game->rows, FALSE);
    }
}

void cleanupGame(Game *game)
{
    int i;

    for (i = 0; i < game->columns; i++) {
        free(game->board[i]);
    }

    free(game->board);
}
