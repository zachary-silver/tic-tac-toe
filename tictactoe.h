#ifndef TICTACTOE_H
#define TICTACTOE_H

typedef enum { X = 'X', O = 'O', Neither = ' ' } Player;
typedef enum { False = 0, True = 1 } Boolean;

typedef struct {
    unsigned XPositions;
    unsigned OPositions;
    signed char score;
} BoardState;

typedef struct {
    Player **board;
    int rows;
    int columns;
    int player1Score;
    int player2Score;
    Player player1;
    Player player2;
} Game;

typedef struct {
    int row;
    int column;
} Play;

void play(Game *game);
void resetBoard(Game *game);
Player getNextPlayer(Player player);
Player findWinner(Game *game);
Boolean isTie(Game *game);
BoardState getBoardState(Game *game);

#endif
