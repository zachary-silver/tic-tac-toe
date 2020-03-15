#include <stdlib.h>

#include "ui.h"
#include "tictactoe.h"
#include "memory.h"
#include "setup.h"

int main(void)
{
    int size;
    Player player, winner;
    Game game;

    size = getBoardSize();
    initializeGame(&game, size, size);

    player = One;
    do {
        resetBoard(&game);

        do {
            makePlay(&game, player);
            printBoard(&game);
            player = getNextPlayer(player);
        } while ((winner = findWinner(&game)) == Neither && !isTie(&game));

        if (winner == One) {
            game.playerOneScore++;
        } else if (winner == Two) {
            game.playerTwoScore++;
        }

        printScore(&game);
    } while (promptToContinue());

    cleanupGame(&game);

    return 0;
}
