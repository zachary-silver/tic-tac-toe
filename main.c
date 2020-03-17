#include "ui.h"
#include "tictactoe.h"
#include "setup.h"

int main(void)
{
    Game game;

    initializeGame(&game, 3);

    game.player1 = getPlayer();
    game.player2 = getNextPlayer(game.player1);

    play(&game);

    return 0;
}
