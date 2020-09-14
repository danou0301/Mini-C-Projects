/**
 * @file battleships.c
 * @author  Dan Boujenah
 * @version 1.0
 * @date 19 Aug 2018
 * @brief the battleships game with a console interface
 */
// ------------------------------ includes --------------------------------

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include "battleships.h"
// -------------------------- const definitions ---------------------------

#define MAX_SIZE 26
#define MIN_SIZE 5
#define HIT_SIGN 'x'
#define MISS_SIGN 'o'
#define HIDDEN_SIGN '_'
#define ASK_COORDINATE "enter coordinates: "
#define INVALID_MOVE "Invalid move, try again.\n"
#define ALREADY_HIT "Already been Hit.\n"
#define HIT "Hit.\n"
#define HIT_AND_SUNK "Hit and sunk.\n"
#define MISS "Miss\n"
#define GAME_OVER "Game over\n"
#define START_MSG "Ready to play\n"
#define ASK_SIZE_MSG "enter board size: "
#define NOT_VALID_SIZE "not valid board size!\n"
#define NOT_FITTING "board size not fitting!\n"
#define TOO_SMALL_SIZE "there is no possible position for the ships! the board is too small"
#define EXIT_GAME "exit"
// ------------------------------ functions -------------------------------

/**
 * print a board to the console
 * @param size board size
 * @param board pointer to a list of list of char, every char corresponding to a
 * coordinate on the board
 */
void printBoard(int size, char**board)
{
    printf(" ");
    for (int i = 1; i <= size; ++i)
    {
        printf(",%d", i);
    }
    printf("\n");

    for (int j = 0; j < size; ++j)
    {
        printf("%c", j + 97);
        for (int k = 0; k < size; ++k)
        {
            printf(" %c", board[j][k]);
        }
        printf("\n");

    }
}

/**
 * initialize the board according to the user size
 * @param size size of the board
 * @param board empty board
 */
void initBoard(int size, char **board)
{
    for (int i = 0; i < size; ++i)
    {
        board[i] = (char*)malloc(sizeof(char) * size);
        if (board[i] == NULL)
        {
            exit(1);
        }
    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            board[i][j] = HIDDEN_SIGN;
        }
    }
}

/**
 * fire on the board
 * @param board the board
 * @param arrBoat a list of boat
 * @param x coordinate x
 * @param y coordinate y
 * @param sinks number of sunk boats
 */
void fire(char ** board, Boat* arrBoat, int x, int y, int *sinks)
{
    int hit = checkHit(arrBoat, x, y);
    if (hit != 0) // we hit a boat
    {
        board[x - 1][y - 1] = HIT_SIGN;
        arrBoat[hit - 1].numOfHit++;

        if (arrBoat[hit - 1].numOfHit == arrBoat[hit - 1].length)
        {
            *sinks = *sinks + 1;
            printf(HIT_AND_SUNK);
        }
        else
        {
            printf(HIT);
        }
    }
    else
    {
        board[x - 1][y - 1] = MISS_SIGN;
        printf(MISS);
    }
}

/**
 * run a battleships game
 * @param size size of the board
 * @param board pointer to a list of list of char, every char corresponding to a
 * coordinate on the board
 * @param arrBoat pointer to a list of boats
 */
void runGame(int size, char **board, Boat* arrBoat)
{
    char str[5];
    char x;
    int y, matches, sinks = 0;

    while (sinks < NUMBER_OF_BOATS)
    {
        printf(ASK_COORDINATE);
        scanf(" %[^\t\n]", str);
        if (strcmp(str, EXIT_GAME) == 0)
        {
            break;
        }
        matches = sscanf(str, "%c %d", &x, &y);
        x -= 'a' - 1 ;

        if (x < 0 || y < 0 || x > size || y > size)
        {
            fprintf(stderr, INVALID_MOVE);
        }
        else if (board[x - 1][y - 1] == MISS_SIGN || board[x - 1][y - 1] == HIT_SIGN)
        {
            fprintf(stderr, ALREADY_HIT);
        }

        else if (matches == 2) // good move
        {
            fire(board, arrBoat, x, y, &sinks);
            printBoard(size, board);
        }
        else
        {
            fprintf(stderr, INVALID_MOVE);
        }

        if (sinks == NUMBER_OF_BOATS)
        {
            printf(GAME_OVER);
        }

    }
}

/**
 * ask to the user the board size
 * @return the right size of the board
 */
int getBoardSize()
{
    int matches, size;
    printf(ASK_SIZE_MSG);
    matches = scanf("%d", &size);

    if (matches != 1)
    {
        fprintf(stderr, NOT_VALID_SIZE);
        exit(1);
    }
    if (size > MAX_SIZE || size < 1)
    {
        fprintf(stderr, NOT_FITTING);
        exit(1);
    }
    else if (size < MIN_SIZE)
    {
        fprintf(stderr, TOO_SMALL_SIZE);
        exit(1);
    }
    else
    {
        return size;
    }
}
/**
 * the main function
 * @return 0 if the program run right
 */
int main()
{
    int size = getBoardSize();
    char ** board;
    board = (char**) malloc(sizeof(char*) * size);
    if (board == NULL)
    {
        exit(1);
    }

    initBoard(size, board);
    Boat arrBoat[NUMBER_OF_BOATS] = {};
    addBoat(arrBoat, size); // add boat

    printBoard(size, board);
    printf(START_MSG);
    runGame(size, board, arrBoat);

    for (int i = 0; i < size; ++i) // free the board
    {
        free(board[i]);
    }
    free(board);
    return 0;
}