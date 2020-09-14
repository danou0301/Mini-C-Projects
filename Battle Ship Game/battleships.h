
#ifndef EX2_BATTLESHIPS_H
#define EX2_BATTLESHIPS_H

#define NUMBER_OF_BOATS 5
#define HORIZONTAL 0
#define VERTICAL 1
#define PLANE_SIZE 5
#define PATROL_SIZE 4
#define BOAT_SIZE 3
#define SUBMARINE_SIZE 3
#define DESTROYER_SIZE 2
#define START_BOARD_X 1
#define START_BOARD_Y 1
/**
 * structure that take the x and y coordinate
 */
typedef struct Coordinate
{
    int x;
    int y;
}Coordinate;

/**
 * structure of the boat
 */
typedef struct Boat
{
    Coordinate coo;
    int length;
    int dir;
    int numOfHit;
}Boat;

/**
 * take a coo and check if a boat is on this Coordinate
 * @param arrBoat pointer to a list of boats
 * @param x x coordinate to check
 * @param y y coordinate to check
 * @return the n0 (1 to 5) of the boat
 */
int checkHit(Boat* arrBoat, int x, int y);

/**
 * add a boat to the board
 * @param arrBoat pointer the a list of boat that already exist
 * @param size size of the board
 */
void addBoat(Boat* arrBoat, int size);

#endif //EX2_BATTLESHIPS_H
