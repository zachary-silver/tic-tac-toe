#include <limits.h>

#include "tictactoe.h"
#include "ui.h"

typedef enum { Minimizer = -1, Tie = 0, Maximizer = 1 } Score;

typedef int (*Optimize)(const int left, const int right);

void playRound(Game *game);
void playTurn(Game *game, Player player);
int minimax(Game *game, Player player, int alpha, int beta);
void minMax(Game *game, Play *play, Player player, int *alpha, int *beta);
void makePlay(Game *game, Player player);
Boolean diagonalWin(Game *game, Player player);
Boolean increasingDiagonalWin(Game *game, Player player);
Boolean decreasingDiagonalWin(Game *game, Player player);
Boolean columnWin(Game *game, int column, Player player);
Boolean rowWin(Game *game, int row, Player player);
Optimize getOptimizer(Player player);
int getInitialScore(Player player);
void resetBoard(Game *game);
int max(int left, int right);
int min(int left, int right);

void play(Game *game)
{
    do {
        playRound(game);

        printScore(game);

        game->player1 = game->player2;
        game->player2 = getNextPlayer(game->player2);
    } while (getContinue());
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

void makePlay(Game *game, Player player)
{
    int row, column, currentScore, bestScore;
    Player nextPlayer;
    Optimize optimize;
    Play play;

    optimize = getOptimizer(player);
    bestScore = getInitialScore(player);
    nextPlayer = getNextPlayer(player);

    for (row = 0; row < game->rows; row++) {
        for (column = 0; column < game->columns; column++) {
            if (game->board[column][row] == Neither) {
                game->board[column][row] = player;

                currentScore = minimax(game, nextPlayer, INT_MIN, INT_MAX);

                if (optimize(currentScore, bestScore) != bestScore) {
                    bestScore = currentScore;
                    play.column = column;
                    play.row = row;
                }

                game->board[column][row] = Neither;
            }
        }
    }

    game->board[play.column][play.row] = player;
}

int minimax(Game *game, Player player, int alpha, int beta)
{
    int row, column;
    Player winner;
    Play play;

    if ((winner = findWinner(game)) != Neither) {
        return winner == X ? Maximizer : Minimizer;
    } else if (isTie(game)) {
        return Tie;
    }

    for (row = 0; row < game->rows; row++) {
        for (column = 0; column < game->columns; column++) {
            if (game->board[column][row] == Neither) {
                play.column = column;
                play.row = row;

                minMax(game, &play, player, &alpha, &beta);

                if (beta <= alpha) {
                    return player == X ? alpha : beta;
                }
            }
        }
    }

    return player == X ? alpha : beta;
}

void minMax(Game *game, Play *play, Player player, int *alpha, int *beta)
{
    int score;

    game->board[play->column][play->row] = player;

    score = minimax(game, getNextPlayer(player), *alpha, *beta);

    if (player == X) {
        *alpha = max(score, *alpha);
    } else {
        *beta = min(score, *beta);
    }

    game->board[play->column][play->row] = Neither;
}

Player findWinner(Game *game)
{
    int row, column;
    Player winner;

    for (row = 0; row < game->rows; row++) {
        winner = game->board[0][row];
        if (winner != Neither && rowWin(game, row, winner)) {
            return winner;
        }
    }

    for (column = 0; column < game->columns; column++) {
        winner = game->board[column][0];
        if (winner != Neither && columnWin(game, column, winner)) {
            return winner;
        }
    }

    winner = game->board[game->columns / 2][game->rows / 2];
    return winner != Neither && diagonalWin(game, winner) ? winner : Neither;
}

Boolean rowWin(Game *game, int row, Player player)
{
    int column;

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
int getInitialScore(Player player)
{
    return player == X ? INT_MIN : INT_MAX;
}

Optimize getOptimizer(Player player)
{
    return player == X ? max : min;
}

Player getNextPlayer(Player player)
{
    return player == X ? O : X;
}

int max(int left, int right)
{
    return left > right ? left : right;
}

int min(int left, int right)
{
    return left < right ? left : right;
}
