#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "ui.h"

#define SPACES 10

int validSpot(Game *game, int row, int column);
int validContinueResponse(char response);
int validPlay(Game *game, Play *play);
int validPlayer(char player);
void printRow(Game *game, int row);
void printSpaces(int spaces);
void printLine(Game *game);
void flushInput(void);

int getContinue(void)
{
    char response = 0;

    do {
        printf("Would you like to continue playing? [Y/N]: ");
        scanf("%c", &response);
        flushInput();
        response = tolower(response);
    } while (!validContinueResponse(response));

    return response == 'y' ? 1 : 0;
}

Player getPlayer(void)
{
    char player = 0;

    do {
        printf("What would you like to play as? [X/O]: ");
        scanf("%c", &player);
        flushInput();
        player = toupper(player);
    } while(!validPlayer(player));

    return player;
}

Play getPlay(Game *game)
{
    Play play;

    do {
        play.row = -1;
        play.column = -1;
        printf("Which row and column would you like to go? ");
        printf("[1-%d] [1-%d]: ", game->rows, game->columns);
        scanf("%d %d", &play.row, &play.column);
        flushInput();
        play.row--;
        play.column--;
    } while (!validPlay(game, &play));

    return play;
}

void printScore(Game *game)
{
    printf("Player1 score: %d\n", game->player1Score);
    printf("Player2 score: %d\n\n", game->player2Score);
}

void printBoard(Game *game)
{
    int row;

    for (row = 0; row < game->rows - 1; row++) {
        printSpaces(SPACES);
        printRow(game, row);
        printSpaces(SPACES);
        printLine(game);
    }
    printSpaces(SPACES);
    printRow(game, row);
    printf("\n\n");
}

void printRow(Game *game, int row)
{
    int column;

    for (column = 0; column < game->columns - 1; column++) {
        printf(" %c |", game->board[column][row]);
    }
    printf(" %c ", game->board[column][row]);
}

void printLine(Game *game)
{
    int i;

    for (i = 0; i < game->columns * 4 - 1; i++) {
        printf("-");
    }
}

void printSpaces(int spaces)
{
    int i;

    printf("\n");
    for (i = 0; i < spaces; i++) {
        printf(" ");
    }
}

int validPlay(Game *game, Play *play)
{
    if (!validSpot(game, play->row, play->column)) {
        printf("Invalid row and or column\n");
        return 0;
    }

    if (game->board[play->column][play->row] != Neither) {
        printf("Spot is already occupied\n");
        return 0;
    }

    return 1;
}

int validSpot(Game *game, int row, int column)
{
    return row < game->rows && row >= 0 &&
           column < game->columns && column >= 0;
}

int validPlayer(char player)
{
    return player == 'X' || player == 'O';
}

int validContinueResponse(char response)
{
    return response == 'y' || response == 'n';
}

void flushInput(void)
{
    int c;

    while ((c = getc(stdin)) != EOF && c != '\n');
}
