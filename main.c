#include "ui.h"
#include "tictactoe.h"
#include "memory.h"
#include "setup.h"

int main(void)
{
    int size;
    Game game;

    size = getBoardSize();
    initializeGame(&game, size, size);

    game.player1 = getPlayer();
    game.player2 = getNextPlayer(game.player1);

    play(&game);
    cleanupGame(&game);

    return 0;
}
