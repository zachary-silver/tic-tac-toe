#include <stdlib.h>

#include "setup.h"
#include "memory.h"

void initializeGame(Game *game, int rows, int columns)
{
    const char *errMessage = "tictactoe: getMemory";
    int i;

    game->board = getMemory(sizeof(Player*) * game->columns, FALSE, errMessage);
    for (i = 0; i < game->columns; i++) {
        game->board[i] = getMemory(sizeof(Player) * game->rows, FALSE,
                                   errMessage);
    }
    game->rows = rows;
    game->columns = columns;
    game->playerOneScore = 0;
    game->playerTwoScore = 0;
}

void cleanupGame(Game *game)
{
    int i;

    for (i = 0; i < game->columns; i++) {
        free(game->board[i]);
    }

    free(game->board);
}
