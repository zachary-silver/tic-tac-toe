#include "ui.h"
#include "tictactoe.h"
#include "memory.h"
#include "setup.h"

void playTurn(Game *game, Player player);
void playRound(Game *game);

int main(void)
{
    int size;
    Game game;

    size = getBoardSize();
    initializeGame(&game, size, size);

    game.player1 = getPlayer();
    game.player2 = getNextPlayer(game.player1);

    do {
        playRound(&game);
        printScore(&game);

        game.player1 = game.player2;
        game.player2 = getNextPlayer(game.player2);
    } while (getContinue());

    cleanupGame(&game);

    return 0;
}

void playRound(Game *game)
{
    Player winner, player = X;

    resetBoard(game);
    printBoard(game);

    do {
        playTurn(game, player);
        player = getNextPlayer(player);
    } while ((winner = findWinner(game)) == Neither && !isTie(game));

    if (winner == game->player1) {
        game->player1Score++;
    } else if (winner == game->player2) {
        game->player2Score++;
    }
}

void playTurn(Game *game, Player player)
{
    Play play;

    if (player == game->player1) {
        play = getPlay(game);
        game->board[play.column][play.row] = player;
    } else {
        makePlay(game, player);
    }

    printBoard(game);
}
