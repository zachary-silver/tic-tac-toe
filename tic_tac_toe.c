// This program allows two players or a single player
// against an AI to play tic tac toe via user input of
// intended row, column, and x or o.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define ERROR_CODE -1
#define NOT_FOUND -2
#define FOUND 2
#define ROWS 3
#define COLS 3
#define TABS 5
#define TRUE 1
#define FALSE 0

int test_input(char input, char board[ROWS][COLS], int row, int col, char team);
int find_critical_spot(char board[ROWS][COLS], int type, int index);
char find_winner(char board[ROWS][COLS]);
void clear_board(char board[ROWS][COLS]);
void display_board(char board[ROWS][COLS]);
void draw_square(char input, int type);
void draw_tabs(int tabs);
void fill_open_spot(char board[ROWS][COLS], char team);
void make_play(char board[ROWS][COLS], char team);

int main(void)
{
    char input = 0, winner = 0,
         player1_team = 0, player2_team = 0, active_team = 0,
         board[ROWS][COLS] = {};
    int row = 0, col = 0, turn = 0, round = 0, auto_player = 0,
        player1_score = 0, player2_score = 0,
        match_over = FALSE, game_over = FALSE;
    time_t seed;
    (void)time(&seed);
    srand((long)seed);

    printf("TIC TAC TOE");
    printf("\n\nPlay against an automatic player? Y/N > ");
    scanf("%c", &input);

    input = tolower(input);
    if (input == 'y')
        auto_player = rand() % 2; // randomly assign auto as player1 or player2
    else
        auto_player = NOT_FOUND;

    turn = rand() % 2; // randomly decide which player goes first
    while (!game_over)
    {
        if (rand() % 2) // randomly assign players to X's or O's
        {
            player1_team = 'x';
            player2_team ='o';
        }
        else
        {
            player1_team = 'o';
            player2_team = 'x';
        }

        round = 0;
        while (!match_over)
        {
            round++;
            turn = (turn + 1) % 2;
            active_team = (turn == 0) ? player1_team : player2_team;

            if (turn == auto_player)
            {
                make_play(board, active_team);
                display_board(board);
                printf("\n");
            }
            else
            {
                do
                {
                    printf("\nPlayer %i, enter with spaces: Row, Column, and %c > ",
                            turn + 1, active_team);
                    scanf("%i %i %c", &row, &col, &input);
                }
                while (test_input(input, board, row, col, active_team) == ERROR_CODE);

                board[row - 1][col - 1] = active_team;
                display_board(board);
                printf("\n");
            }

            winner = find_winner(board);
            if (winner)
            {
                printf("\n%c wins!", winner);

                if (turn == 0)
                    player1_score += 1;
                else
                    player2_score += 1;

                printf("\nPlayer1 total wins: %i\nPlayer2 total wins: %i",
                        player1_score, player2_score);

                match_over = TRUE;
            }
            else if (round == 9)
            {
                printf("\nGame is a draw!\n");

                printf("\nPlayer1 total wins: %i\nPlayer2 total wins: %i",
                        player1_score, player2_score);

                match_over = TRUE;
            }
        }

        printf("\nNew game? Y/N >");
        scanf(" %c", &input);

        if (tolower(input) == 'n')
        {
            game_over = TRUE;
        }
        else
        {
            match_over = FALSE;

            clear_board(board);
        }
    }
    printf("\n");

    return 0;
}

void announce_play(int row, int col, char team)
{
    printf("\nAuto Player entered: Row %i, Column %i, and %c\n",
                    row + 1, col + 1, team);
}

void make_play(char board[ROWS][COLS], char team)
{
    int xrows[3] = {}, xcols[3] = {}, xdiags[2] = {},
        orows[3] = {}, ocols[3] = {}, odiags[2] = {},
        spot = NOT_FOUND, play_made = FALSE,
        i, j, k;

    // sum the number of x's and o's in each row, column, and diagonal
    for (i = 0, k = ROWS - 1; i < ROWS; i++, k--)
    {
        for (j = 0; j < COLS; j++)
        {
            if (board[i][j] == 'x')
                xrows[i] += 1;
            else if (board[i][j] == 'o')
                orows[i] += 1;

            if (board[j][i] == 'x')
                xcols[i] += 1;
            else if (board[j][i] == 'o')
                ocols[i] += 1;
        }

        if (board[i][i] == 'x')
            xdiags[0] += 1;
        else if (board[i][i] == 'o')
            odiags[0] += 1;

        if (board[i][k] == 'x')
            xdiags[1] += 1;
        else if (board[i][k] == 'o')
            odiags[1] += 1;
    }

    // search for and fill a row or column with 2 x's or o's and an open spot
    for (i = 0; i < ROWS && !play_made; i++)
    {
        if ((xrows[i] == ROWS - 1 || orows[i] == ROWS - 1) &&
            (find_critical_spot(board, 0, i) != NOT_FOUND))
        {
            spot = find_critical_spot(board, 0, i);
            board[i][spot] = team;
            announce_play(i, spot, team);

            play_made = TRUE;
        }
        else if ((xcols[i] == ROWS - 1 || ocols[i] == ROWS - 1) &&
                 (find_critical_spot(board, 1, i) != NOT_FOUND))
        {
            spot = find_critical_spot(board, 1, i);
            board[spot][i] = team;
            announce_play(spot, i, team);

            play_made = TRUE;
        }
    }

    // or, search for and fill a diagonal with 2 x's or o's and an open spot
    if (!play_made)
    {
        if ((xdiags[0] == ROWS - 1 || odiags[0] == ROWS - 1) &&
            (find_critical_spot(board, 2, 0) != NOT_FOUND))
        {
            spot = find_critical_spot(board, 2, 0);
            board[spot][spot] = team;
            announce_play(spot, spot, team);

            play_made = TRUE;
        }
        else if ((xdiags[1] == ROWS - 1 || odiags[1] == ROWS - 1) &&
                 (find_critical_spot(board, 2, 1) != NOT_FOUND))
        {
            spot = find_critical_spot(board, 2, 1);
            board[spot][ROWS - 1 - spot] = team;
            announce_play(spot, ROWS - 1 - spot, team);

            play_made = TRUE;
        }
    }

    // if no row, column, or diagonal with 2 x's or o's and an open spot, randomly fill one
    if (!play_made)
        fill_open_spot(board, team);
}

int find_critical_spot(char board[ROWS][COLS], int type, int index)
{
    int spot = NOT_FOUND,
        i, j;

    switch (type)
    {
        case 0: // row
            for (i = 0; i < ROWS; i++)
                if (board[index][i] == 0)
                    spot = i;
            break;
        case 1: // column
            for (i = 0; i < COLS; i++)
                if (board[i][index] == 0)
                    spot = i;
            break;
        case 2: // diagonal
            if (index == 0) // decreasing diagonal
            {
                for (i = 0; i < ROWS; i++)
                    if (board[i][i] == 0)
                        spot = i;
            }
            else // increasing diagonal
            {
                for (i = 0, j = ROWS - 1; i < ROWS; i++, j--)
                    if (board[i][j] == 0)
                        spot = i;
            }
            break;
    }

    return spot;
}

int fill_empty_lane(char board[ROWS][COLS], char team)
{
    int empty_rows[3] = {}, empty_cols[3] = {}, empty_diags[2] = {},
        spot = NOT_FOUND,
        i, j, k;

    // sum the number of empty spot in each row and column
    for (i = 0, k = ROWS - 1; i < ROWS; i++, k--)
    {
        for (j = 0; j < COLS; j++)
        {
            if (board[i][j] == 0)
                empty_rows[i] += 1;

            if (board[j][i] == 0)
                empty_cols[i] += 1;
        }

        if (board[i][i] == 0)
            empty_diags[0] += 1;

        if (board[i][k] == 0)
            empty_diags[1] += 1;
    }

    // test for empty rows and columns
    for (i = 0; i < ROWS && spot == NOT_FOUND; i++)
    {
        if (empty_rows[i] == COLS)
        {
            spot = rand() % COLS;
            board[i][spot] = team;
            announce_play(i, spot, team);
        }
        else if (empty_cols[i] == ROWS)
        {
            spot = rand() % ROWS;
            board[spot][i] = team;
            announce_play(spot, i, team);
        }
    }

    // test for empty diagonals
    if (spot == NOT_FOUND)
    {
        if (empty_diags[0] == ROWS)
        {
            spot = rand() % ROWS;
            board[spot][spot] = team;
            announce_play(spot, spot, team);
        }
        else if (empty_diags[1] == ROWS)
        {
            spot = rand() % ROWS;
            board[spot][ROWS - 1 - spot] = team;
            announce_play(spot, ROWS - 1 - spot, team);
        }
    }

    return spot;
}

void fill_open_spot(char board[ROWS][COLS], char team)
{
    int spot = NOT_FOUND;

    // fill a randomly selected spot in an empty lane if one exists
    spot = fill_empty_lane(board, team);

    // otherwise, fill a randomly selected open spot
    while (spot == NOT_FOUND)
    {
        for (int i = (rand() % ROWS); i < ROWS && spot == NOT_FOUND; i++)
        {
            for (int j = (rand() % COLS); j < COLS && spot == NOT_FOUND; j++)
            {
                if (board[i][j] == 0)
                {
                    board[i][j] = team;
                    announce_play(i, j, team);

                    spot = FOUND;
                }
            }
        }
    }
}

int test_input(char input, char board[ROWS][COLS], int row, int col, char team)
{
    int result = 0;
    input = tolower(input);

    if (row < 1 || row > 3)
    {
        printf("\nInvalid row entered");
        result = ERROR_CODE;
    }
    else if (col < 1 || col > 3)
    {
        printf("\nInvalid column entered");
        result = ERROR_CODE;
    }
    else if (input != team)
    {
        printf("\nInvalid character, %c required", team);
        result = ERROR_CODE;
    }
    else if (board[row - 1][col - 1] != 0)
    {
        printf("\nRow %i, Column %i already taken", row, col);
        result = ERROR_CODE;
    }

    return result;
}

void clear_board(char board[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = 0;
}

void display_board(char board[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
    {
        printf("\n");
        draw_tabs(TABS);

        for (int j = 0; j < COLS; j++)
            draw_square(board[i][j], j);

        if (i < ROWS - 1) // don't want to print a line after the last row
        {
            printf("\n");
            draw_tabs(TABS);
            printf("-----------");
        }
    }
}

void draw_square(char input, int type)
{
    switch (type)
    {
        case 0:
        case 1:
            printf(" %*c |", (input == 0) ? 2 : 1, input);
            break;
        case 2:
            printf(" %*c ", (input == 0) ? 2 : 1, input);
            break;
    }
}

void draw_tabs(int tabs)
{
    for (int i = 0; i < tabs; i++)
        printf("\t");
}

char find_winner(char board[ROWS][COLS])
{
    int xrows[3] = {}, xcols[3] = {},
        orows[3] = {}, ocols[3] = {},
        diags[2] = {},
        i, j, k;
    char winner = 0,
         // only the team in the center spot can win diagonally
         potential_winner = board[ROWS / 2][COLS / 2];

    // sum the number of x's and o's in each row, column, and diagonal
    for (i = 0, k = ROWS - 1; i < ROWS; i++, k--)
    {
        for (j = 0; j < COLS; j++)
        {
            if (board[i][j] == 'x')
                xrows[i] += 1;
            else if (board[i][j] == 'o')
                orows[i] += 1;

            if (board[j][i] == 'x')
                xcols[i] += 1;
            else if (board[j][i] == 'o')
                ocols[i] += 1;
        }

        if (board[i][i] == potential_winner)
            diags[0] += 1;

        if (board[i][k] == potential_winner)
            diags[1] += 1;
    }

    // test for winning row or column
    for (i = 0; i < ROWS; i++)
    {
        if (xrows[i] == ROWS || xcols[i] == COLS)
            winner = 'x';
        else if (orows[i] == ROWS || ocols[i] == COLS)
            winner = 'o';
    }

    // test for winning diagonal
    if (potential_winner != 0 && (diags[0] == ROWS || diags[1] == ROWS))
            winner = potential_winner;

    return winner;
}
