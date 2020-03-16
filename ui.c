#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "ui.h"

#define SPACES 10
#define MINSIZE 3
#define MAXSIZE 3

int validSpot(Game *game, int row, int column);
int validContinueResponse(char response);
int validPlay(Game *game, Play *play);
int validPlayer(char player);
int validBoardSize(int size);
void printRow(Game *game, int row);
void printSpaces(int spaces);
void printLine(Game *game);
void handleInvalidSize(void);
void flushInput(void);

int getContinue(void)
{
    char response = 0;

    do {
        flushInput();
        printf("Would you like to continue playing? [Y/N]: ");
        scanf("%c", &response);
        response = tolower(response);
    } while (!validContinueResponse(response));

    return response == 'y' ? 1 : 0;
}

Player getPlayer(void)
{
    char player = 0;

    do {
        flushInput();
        printf("What would you like to play as? [X/O]: ");
        scanf("%c", &player);
        player = toupper(player);
    } while(!validPlayer(player));

    return player;
}

Play getPlay(Game *game)
{
    Play play;

    do {
        flushInput();
        play.row = -1;
        play.column = -1;
        printf("Where would you would like to go? ");
        printf("[1-%d] [1-%d]: ", game->rows, game->columns);
        scanf("%d %d", &play.row, &play.column);
        play.row--;
        play.column--;
    } while (!validPlay(game, &play));

    return play;
}

int getBoardSize(void)
{
    int size;

    do {
        printf("How big do you want the game board to be? ");
        printf("[%d-%d]: ", MINSIZE, MAXSIZE);
        scanf("%d", &size);
    } while (!validBoardSize(size));

    return size;
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

int validBoardSize(int size)
{
    return size >= MINSIZE && size <= MAXSIZE;
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
