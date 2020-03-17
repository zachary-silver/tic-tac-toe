#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "tictactoe.h"
#include "hashTable.h"
#include "memory.h"
#include "ui.h"

typedef enum { Minimizer = -1, Tie = 0, Maximizer = 1 } Score;

typedef int (*MinMax)(const int left, const int right);

int minimax(Game *game, Player player, void *boardStates, int alpha, int beta);
void makePlay(Game *game, Player player, void *boardStates);
Boolean diagonalWin(Game *game, Player player);
Boolean increasingDiagonalWin(Game *game, Player player);
Boolean decreasingDiagonalWin(Game *game, Player player);
Boolean columnWin(Game *game, int column, Player player);
Boolean rowWin(Game *game, int row, Player player);
MinMax getMinMax(Player player);
int getBestScore(Player player);
int max(int left, int right);
int min(int left, int right);

unsigned hashBoardState(const BoardState *boardState)
{
    return (((boardState->XPositions + boardState->OPositions) *
           (boardState->XPositions + boardState->OPositions + 1)) /
           2) + boardState->OPositions;
}

int compareBoardState(const BoardState *left, const BoardState *right)
{
    return left->XPositions != right->XPositions ||
           left->OPositions != right->OPositions;
}

void playTurn(Game *game, Player player, void *boardStates)
{
    Play play;

    if (player == game->player1) {
        play = getPlay(game);
        game->board[play.column][play.row] = player;
    } else {
        makePlay(game, player, boardStates);
    }

    printBoard(game);
}

void playRound(Game *game, void *boardStates)
{
    Player winner, player = X;

    resetBoard(game);
    printBoard(game);

    do {
        playTurn(game, player, boardStates);
        player = getNextPlayer(player);
    } while ((winner = findWinner(game)) == Neither && !isTie(game));

    if (winner == game->player1) {
        game->player1Score++;
    } else if (winner == game->player2) {
        game->player2Score++;
    }
}

void *getHashTable(void)
{
    int numOfSizes = 2;
    unsigned sizes[] = {
        60013, /* covers all 3x3 game states with 0.5 load factor */
        74051161 /* roughly 2 gigabytes worth of entries */
    };
    HTFunctions functions = {
        (HTHash)hashBoardState,
        (HTCompare)compareBoardState,
        NULL
    };

    return htCreate(&functions, sizes, numOfSizes, 0.5);
}

void play(Game *game)
{
    void *boardStates = getHashTable();

    do {
        playRound(game, boardStates);

        printScore(game);

        game->player1 = game->player2;
        game->player2 = getNextPlayer(game->player2);
    } while (getContinue());

    htDestroy(boardStates);
}

void setPosition(Game *game, unsigned *positions, int row, int column)
{
    *positions |= ((long)1) << (row * game->columns + column);
}

BoardState getBoardState(Game *game)
{
    BoardState state = { 0 };
    int row, column;

    for (row = 0; row < game->rows; row++) {
        for (column = 0; column < game->columns; column++) {
            if (game->board[column][row] == X) {
                setPosition(game, &state.XPositions, row, column);
            } else if (game->board[column][row] == O) {
                setPosition(game, &state.OPositions, row, column);
            }
        }
    }

    return state;
}

BoardState *createBoardState(BoardState *state)
{
    BoardState *boardState = getMemory(sizeof(BoardState), FALSE);

    boardState->XPositions = state->XPositions;
    boardState->OPositions = state->OPositions;
    boardState->score = state->score;

    return boardState;
}

void makePlay(Game *game, Player player, void *boardStates)
{
    int row, column, currentScore, bestScore;
    Player nextPlayer;
    BoardState state;
    MinMax minMax;
    HTEntry boardStateEntry;
    Play play;

    minMax = getMinMax(player);
    bestScore = getBestScore(player);
    nextPlayer = getNextPlayer(player);

    for (row = 0; row < game->rows; row++) {
        for (column = 0; column < game->columns; column++) {
            if (game->board[column][row] == Neither) {
                game->board[column][row] = player;

                state = getBoardState(game);
                boardStateEntry = htLookUp(boardStates, &state);
                if (boardStateEntry.data == NULL) {
                    currentScore = minimax(game, nextPlayer, boardStates,
                                           INT_MIN, INT_MAX);
                    state.score = currentScore;
                    htAdd(boardStates, createBoardState(&state));
                } else {
                    currentScore = ((BoardState*)(boardStateEntry.data))->score;
                }

                if (minMax(currentScore, bestScore) != bestScore) {
                    bestScore = currentScore;
                    play.row = row;
                    play.column = column;
                }

                game->board[column][row] = Neither;
            }
        }
    }

    game->board[play.column][play.row] = player;
}

int minimax(Game *game, Player player, void *boardStates, int alpha, int beta)
{
    int row, column, score;
    Player nextPlayer, winner;
    HTEntry boardStateEntry;
    BoardState state;

    if ((winner = findWinner(game)) != Neither) {
        return winner == X ? Maximizer : Minimizer;
    } else if (isTie(game)) {
        return Tie;
    }

    nextPlayer = getNextPlayer(player);

    for (row = 0; row < game->rows; row++) {
        for (column = 0; column < game->columns; column++) {
            if (game->board[column][row] == Neither) {
                game->board[column][row] = player;

                state = getBoardState(game);
                boardStateEntry = htLookUp(boardStates, &state);
                if (boardStateEntry.data == NULL) {
                    score = minimax(game, nextPlayer, boardStates, alpha, beta);
                    state.score = score;
                    htAdd(boardStates, createBoardState(&state));
                } else {
                    score = ((BoardState*)(boardStateEntry.data))->score;
                }

                if (player == X) {
                    alpha = max(score, alpha);
                } else {
                    beta = min(score, beta);
                }

                game->board[column][row] = Neither;

                if (beta <= alpha) {
                    return player == X ? alpha : beta;
                }
            }
        }
    }

    return player == X ? alpha : beta;
}

Player findWinner(Game *game)
{
    int row, column;
    Player winner;

    for (row = 0; row < game->rows; row++) {
        winner = game->board[0][row];
        if (rowWin(game, row, winner)) {
            return winner;
        }
    }

    for (column = 0; column < game->columns; column++) {
        winner = game->board[column][0];
        if (columnWin(game, column, winner)) {
            return winner;
        }
    }

    winner = game->board[game->columns / 2][game->rows / 2];
    return diagonalWin(game, winner) ? winner : Neither;
}

Boolean rowWin(Game *game, int row, Player player)
{
    int column;

    if (player == Neither) {
        return False;
    }

    for (column = 0; column < game->columns; column++) {
        if (game->board[column][row] != player) {
            return False;
        }
    }

    return True;
}

Boolean columnWin(Game *game, int column, Player player)
{
    int row;

    if (player == Neither) {
        return False;
    }

    for (row = 0; row < game->rows; row++) {
        if (game->board[column][row] != player) {
            return False;
        }
    }

    return True;
}

Boolean diagonalWin(Game *game, Player player)
{
    return decreasingDiagonalWin(game, player) ||
           increasingDiagonalWin(game, player);
}

Boolean decreasingDiagonalWin(Game *game, Player player)
{
    int row, column;

    if (player == Neither) {
        return False;
    }

    for (row = 0, column = 0; row < game->rows; row++, column++) {
        if (game->board[column][row] != player) {
            return False;
        }
    }

    return True;
}

Boolean increasingDiagonalWin(Game *game, Player player)
{
    int row, column;

    if (player == Neither) {
        return False;
    }

    for (row = game->rows - 1, column = 0; row >= 0; row--, column++) {
        if (game->board[column][row] != player) {
            return False;
        }
    }

    return True;
}

Boolean isTie(Game *game)
{
    int row, column;

    for (row = 0; row < game->rows; row++) {
        for (column = 0; column < game->columns; column++) {
            if (game->board[column][row] == Neither) {
                return False;
            }
        }
    }

    return True;
}

void resetBoard(Game *game)
{
    int row, column;

    for (row = 0; row < game->rows; row++) {
        for (column = 0; column < game->columns; column++) {
            game->board[column][row] = Neither;
        }
    }
}

/*
 * X is the maximizing player and O is the minimizing player.
 * The starting best score should be the worst for each.
 */
int getBestScore(Player player)
{
    return player == X ? INT_MIN : INT_MAX;
}

int max(int left, int right)
{
    return left > right ? left : right;
}

int min(int left, int right)
{
    return left < right ? left : right;
}

MinMax getMinMax(Player player)
{
    return player == X ? max : min;
}

Player getNextPlayer(Player player)
{
    return player == X ? O : X;
}
