#include "ui.h"
#include "tictactoe.h"
#include "memory.h"
#include "setup.h"

int main(void)
{
    int size;
    Player player, winner;
    Play play;
    Game game;

    size = getBoardSize();
    initializeGame(&game, size, size);

    game.player1 = getPlayer();
    game.player2 = getNextPlayer(game.player1);

    do {
        player = X;
        resetBoard(&game);
        printBoard(&game);

        do {
            if (player == game.player1) {
                play = getPlay(&game);
                game.board[play.column][play.row] = player;
            } else {
                makePlay(&game, player);
            }
            printBoard(&game);
            player = getNextPlayer(player);
        } while ((winner = findWinner(&game)) == Neither && !isTie(&game));

        if (winner == game.player1) {
            game.player1Score++;
        } else if (winner == game.player2) {
            game.player2Score++;
        }

        printScore(&game);

        game.player2 = game.player1;
        game.player1 = getNextPlayer(game.player1);
    } while (getContinue());

    cleanupGame(&game);

    return 0;
}
