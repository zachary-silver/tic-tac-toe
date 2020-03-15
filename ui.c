#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "ui.h"

#define SPACES 10
#define MINSIZE 3
#define MAXSIZE 3

void printRow(Game *game, int row);
void printSpaces(int spaces);
void printLine(Game *game);
void handleInvalidSize(void);
void flushInput(void);

int promptToContinue(void)
{
    char response;

    do {
        flushInput();
        printf("Would you like to continue playing? Y/N: ");
        scanf("%c", &response);
        response = tolower(response);
    } while (response != 'y' && response != 'n');

    return response == 'y' ? 1 : 0;
}

int getBoardSize(void)
{
    int size;

    printf("Please enter the desired size of the game board: ");
    if (scanf("%d", &size) != 1 || size < MINSIZE || size > MAXSIZE) {
        handleInvalidSize();
    }

    return size;
}

void printScore(Game *game)
{
    printf("Player one score: %d\n", game->playerOneScore);
    printf("Player two score: %d\n\n", game->playerTwoScore);
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

void flushInput(void)
{
    int c;

    while ((c = getc(stdin)) != EOF && c != '\n');
}

void handleInvalidSize(void)
{
    fprintf(stderr, "Size must be an integer from %d to %d\n",
            MINSIZE, MAXSIZE);
    exit(EXIT_FAILURE);
}
