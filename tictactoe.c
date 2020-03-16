#include <limits.h>

#include "tictactoe.h"

typedef enum { Minimizer = -1, Tie = 0, Maximizer = 1 } Score;

typedef int (*MinMax)(const int left, const int right);

int minimax(Game *game, Player player);
Boolean diagonalWin(Game *game, Player player);
Boolean increasingDiagonalWin(Game *game, Player player);
Boolean decreasingDiagonalWin(Game *game, Player player);
Boolean columnWin(Game *game, int column, Player player);
Boolean rowWin(Game *game, int row, Player player);
MinMax getMinMax(Player player);
int getBestScore(Player player);
int max(int left, int right);
int min(int left, int right);

void makePlay(Game *game, Player player)
{
    int row, column, currentScore, bestScore;
    Player nextPlayer;
    MinMax minMax;
    Play play;

    minMax = getMinMax(player);
    bestScore = getBestScore(player);
    nextPlayer = getNextPlayer(player);

    for (row = 0; row < game->rows; row++) {
        for (column = 0; column < game->columns; column++) {
            if (game->board[column][row] == Neither) {
                game->board[column][row] = player;
                currentScore = minimax(game, nextPlayer);
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

int minimax(Game *game, Player player)
{
    int row, column, currentScore, bestScore;
    Player nextPlayer, winner;
    MinMax minMax;

    if ((winner = findWinner(game)) != Neither) {
        return winner == X ? Maximizer : Minimizer;
    } else if (isTie(game)) {
        return Tie;
    }

    minMax = getMinMax(player);
    bestScore = getBestScore(player);
    nextPlayer = getNextPlayer(player);

    for (row = 0; row < game->rows; row++) {
        for (column = 0; column < game->columns; column++) {
            if (game->board[column][row] == Neither) {
                game->board[column][row] = player;
                currentScore = minimax(game, nextPlayer);
                bestScore = minMax(currentScore, bestScore);
                game->board[column][row] = Neither;
            }
        }
    }

    return bestScore;
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
