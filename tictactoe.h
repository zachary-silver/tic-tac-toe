#ifndef TICTACTOE_H
#define TICTACTOE_H

typedef enum { One = 'X', Two = 'O', Neither = ' ' } Player;
typedef enum { False = 0, True = 1 } Boolean;

typedef struct {
    Player **board;
    int rows;
    int columns;
    int playerOneScore;
    int playerTwoScore;
} Game;

void makePlay(Game *game, Player player);
void resetBoard(Game *game);
Player getNextPlayer(Player player);
Player findWinner(Game *game);
Boolean isTie(Game *game);

#endif
