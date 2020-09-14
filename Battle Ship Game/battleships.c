/**
 * @file battleships.c
 * @author  Dan Boujenah
 * @version 1.0
 * @date 19 Aug 2018
 * @brief logical and data structures of the battleships game
 */

// ------------------------------ includes --------------------------------
#include "battleships.h"
#include <stdlib.h>

// ------------------------------ functions -------------------------------
/**
 * choose a random number between a and b
 * @param a lower bound
 * @param b upper bound
 * @return the random number
 */
int boundRand(int a, int b)
{
    return rand() % (b + 1 - a) + a;

}

/**
 * take a coo and check if a boat is on this Coordinate
 * @param arrBoat pointer to a list of boats
 * @param x x coordinate to check
 * @param y y coordinate to check
 * @return the n0 (1 to 5) of the boat
 */
int checkHit(Boat* arrBoat, int x, int y)
{
    for (int i = 0; i < NUMBER_OF_BOATS; ++i)
    {
        for (int j = 0; j < arrBoat[i].length; ++j)
        {
            if (arrBoat[i].dir == HORIZONTAL)
            {
                if (arrBoat[i].coo.x + j == x && arrBoat[i].coo.y == y)
                {
                    return i + 1;
                }
            }
            else if (arrBoat[i].dir == VERTICAL)
            {
                if (arrBoat[i].coo.x == x && arrBoat[i].coo.y + j == y)
                {
                    return i + 1;
                }
            }

        }
    }
    return 0;
}
/**
 * check if a boat touch an other boat
 * @param arrBoat pointer to a list of boat
 * @param x start coordinate x of the new boat
 * @param y start coordinate y of the new boat
 * @param dir this direction
 * @param length this length
 * @return 0 if no collision or 1 if there is  a collision
 */
int checkBoatOverlap(Boat* arrBoat, int x, int y, int dir, int length)
{
    if(dir == HORIZONTAL)
    {
        for (int i = 0; i < length; ++i)
        {
            if (checkHit(arrBoat, x + i, y) != 0)
            {
                return 1;
            }
        }
    }
    else
    {
        for (int i = 0; i < length; ++i)
        {
            if (checkHit(arrBoat, x , y + i) != 0)
            {
                return 1;
            }
        }
    }
    return 0;

}
/**
 * add a boat to the board
 * @param arrBoat pointer the a list of boat that already exist
 * @param size size of the board
 */
void addBoat(Boat* arrBoat, int size)
{
    int lenBoat[NUMBER_OF_BOATS] = {PLANE_SIZE, PATROL_SIZE, BOAT_SIZE, SUBMARINE_SIZE, DESTROYER_SIZE};

    for (int k = 0; k < NUMBER_OF_BOATS; ++k)
    {
        int flag = 0;
        int start_x = boundRand(START_BOARD_X, size);
        int start_y = boundRand(START_BOARD_Y, size);
        int dir = boundRand(HORIZONTAL, VERTICAL);

        while (flag == 0)
        {

            if (dir == HORIZONTAL)
            {
                if (start_x + lenBoat[k] <= size + 1 && checkBoatOverlap(arrBoat, start_x, start_y,
                    dir, lenBoat[k]) == 0)
                {
                    flag = 1;
                }
            }
            else if(dir == VERTICAL)
            {
                if (start_y + lenBoat[k] <= size + 1 && checkBoatOverlap(arrBoat, start_x, start_y,
                    dir, lenBoat[k]) == 0)
                {
                    flag = 1;
                }
            }
            if (flag == 0)
            {
                start_x = boundRand(START_BOARD_X, size);
                start_y = boundRand(START_BOARD_Y, size);
                dir = boundRand(HORIZONTAL, VERTICAL);
            }
            else
            { // initialize the boat
                arrBoat[k].dir = dir;
                arrBoat[k].length = lenBoat[k];
                arrBoat[k].coo.x = start_x;
                arrBoat[k].coo.y = start_y;
            }
        }
    }

}