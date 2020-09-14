/**
 * @file calculator.c
 * @author  Dan Boujenah
 * @version 1.0
 * @date 25 Aug 2018
 * @brief calculate temperature of each coordinate on a grid
 */

// ------------------------------ includes --------------------------------
#include <stdio.h>
#include "calculator.h"
// ------------------------------ functions -------------------------------
/**
 * find the neighbour of (x,y) if it's cyclic
 * @param grid the grid
 * @param right right neighbour
 * @param top top neighbour
 * @param left left neighbour
 * @param bottom bottom neighbour
 * @param x x coordinate
 * @param y y coordinate
 * @param n width
 * @param m length
 */
void cyclic(double **grid, double *right, double *top, double *left, double *bottom, int x, int y, int n, int m)
{
    if (x == 0)
    {
        *top = grid[n - 1][y];
    }
    else
    {
        *top = grid[x - 1][y];
    }
    if (y == 0)
    {
        *left = grid[x][m - 1];
    }
    else
    {
        *left = grid[x][y - 1];
    }
    if (x == n - 1)
    {
        *bottom = grid[0][y];
    }
    else
    {
        *bottom = grid[x + 1][y];
    }
    if (y == m - 1)
    {
        *right = grid[x][0];
    }
    else
    {
        *right = grid[x][y + 1];
    }
}

/**
 * find the neighbour of (x,y) if it's cyclic
 * @param grid the grid
 * @param right right neighbour
 * @param top top neighbour
 * @param left left neighbour
 * @param bottom bottom neighbour
 * @param x x coordinate
 * @param y y coordinate
 * @param n width
 * @param m length
 */
void notCyclic(double **grid, double *right, double *top, double *left, double *bottom, int x, int y, int n, int m)
{
    if (x == 0)
    {
        *top = 0;
    }
    else
    {
        *top = grid[x - 1][y];
    }
    if (y == 0)
    {
        *left = 0;
    }
    else
    {
        *left = grid[x][y - 1];
    }
    if (x == n - 1)
    {
        *bottom = 0;
    }
    else
    {
        *bottom = grid[x + 1][y];
    }
    if (y == m - 1)
    {
        *right = 0;
    }
    else
    {
        *right = grid[x][y + 1];
    }

}

/**
 * calculate for each points on the grid the new heat
 * @param function heat function
 * @param grid the grid
 * @param n width
 * @param m length
 * @param sources list of sources
 * @param num_sources number of sources
 * @param is_cyclic
 * @return the sum of the points on the grid
 */
double calculateOne(diff_func function, double **grid, int n, int m, source_point *sources, size_t num_sources,
                    int is_cyclic)
{
    double sum = 0;
    int flagCalculate = 1;
    double right, top, left, bottom;
    for (int y = 0; y < m; ++y)
    {
        for (int x = 0; x < n; ++x)
        {
            for (int i = 0; i < (int) num_sources; ++i)
            { // check if the point is a source
                if (sources[i].x == x && sources[i].y == y)
                {
                    flagCalculate = 0;
                    break;
                }
            }
            if (flagCalculate)
            {// calculate if the point is not a source
                if (!is_cyclic)
                {
                    notCyclic(grid, &right, &top, &left, &bottom, x, y, n, m);

                }
                else
                { // if it's cyclic
                    cyclic(grid, &right, &top, &left, &bottom, x, y, n, m);

                }
                grid[x][y] = function(grid[x][y], right, top, left, bottom);
            }
            sum += grid[x][y];
            flagCalculate = 1;
        }
    }
    return sum;
}

/**
 * Calculator function. Applies the given function to every point in the grid iteratively for n_iter loops, or until the
 * cumulative difference is below terminate (if n_iter is 0).
 * @param function function that calculate the temperature of a coordinate
 * @param grid pointer of pointer of value (double) of temperature
 * @param n width of the grid
 * @param m length of the grid
 * @param sources list of the sources points
 * @param num_sources number of sources
 * @param terminate when terminate
 * @param n_iter number of iteration
 * @param is_cyclic 0 if it's not cyclic and cyclic otherwise
 * @return
 */
double calculate(diff_func function, double **grid, size_t n, size_t m, source_point *sources, size_t num_sources,
                 double terminate, unsigned int n_iter, int is_cyclic)
{
    double sum = 0, previousSum = 0;
    sum = calculateOne(function, grid, (int) n, (int) m, sources, num_sources, is_cyclic);
    double delta = sum - previousSum;

    if (n_iter > 0)
    { // if it's cyclic
        for (int i = 0; i < (int) n_iter - 1; ++i)
        {
            previousSum = sum;
            sum = calculateOne(function, grid, (int) n, (int) m, sources, num_sources, is_cyclic);
            delta = sum - previousSum;
        }
    }
    else
    {
        while (delta > terminate || -delta > terminate)
        {
            previousSum = sum;
            sum = calculateOne(function, grid, (int) n, (int) m, sources, num_sources, is_cyclic);
            delta = sum - previousSum;
        }
    }


    return delta;
}

