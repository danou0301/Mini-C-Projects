/**
 * @file reader.c
 * @author  Dan Boujenah
 * @version 1.0
 * @date 25 Aug 2018
 * @brief read the input and print the result
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"
#include "heat_eqn.h"

// ------------------------------ includes --------------------------------

#define INIT_SIZE 2
#define REALLOC_FACTOR 2
#define ERROR_MEMORY "Memory allocation failed."
#define SEPARATOR "----\n"
#define ERROR_SIZE "Error with file format: grid size."
#define ERROR_TERMINATE "Error with file format: termination value."
#define ERROR_N_ITER "Error with file format: n_iter."
#define ERROR_CYCLIC "Error with file format: is_cyclic."
#define ERROR_SOURCE "Error with file format: source point."
#define ERROR_OUT_OF_RANGE "Segmentation fault"

// ------------------------------ functions -------------------------------
/**
 * free the grid list of list
 * @param grid the grid
 * @param n number of list
 */
void freeGrid(double ***grid, int n)
{
    for (int i = 0; i < n; ++i)
    {
        free((*grid)[i]);
    }
    free(*grid);
}

/**
 * free memory, close the file and exit
 * @param line input line
 * @param grid the grid
 * @param n width
 * @param listSources list of heat source
 * @param fp file
 */
void closeAndFree(char *line, double ***grid, int n, source_point *listSources, FILE *fp)
{
    if (grid != NULL)
    {
        freeGrid(grid, n);
    }
    if (line != NULL)
    {
        free(line);
    }
    if (listSources != NULL)
    {
        free(listSources);
    }
    if (fp != NULL)
    {
        fclose(fp);
    }
    exit(1);
}

/**
 * initialization of the grid with 0 on each coordinate
 * @param grid pointer of pointer of value (double) of temperature
 * @param n width of the grid
 * @param m length of the grid
 * @return the grid
 */
int initGrid(double ***grid, int n, int m)
{
    *grid = (double **) malloc(sizeof(double *) * n);
    if (*grid == NULL)
    {
        free(*grid);
        return 1;
    }
    for (int i = 0; i < n; ++i)
    {
        (*grid)[i] = (double *) calloc((size_t) m, sizeof(double));
        if ((*grid)[i] == NULL)
        {
            freeGrid(grid, i);
            return 1;
        }
    }
    return 0;
}

/**
 * Parse the source file
 * @param line input line
 * @param fp the file
 * @param n width
 * @param m length
 * @param grid the grid
 * @param listSources list of source
 * @param numSource number od sources
 * @param initSize
 * @return 0 if it's work and 1 if not
 */
int findSources(char *line, FILE *fp, int n, int m, double ***grid, source_point **listSources, int *numSource,
                int initSize)
{
    int x, y;
    double value;
    size_t len = 0;

    getline(&line, &len, fp);
    while (strcmp(SEPARATOR, line) != 0)
    {
        if (sscanf(line, "%d, %d, %lf", &x, &y, &value) != 3)
        {
            fprintf(stderr, ERROR_SOURCE);
            free(line);
            return 1;
        }
        if (x >= n || y >= m)
        { // check bad input
            fprintf(stderr, ERROR_OUT_OF_RANGE);
            free(line);
            return 1;
        }

        if (*numSource >= initSize)
        {
            initSize *= REALLOC_FACTOR;
            *listSources = (source_point *) realloc(*listSources, sizeof(source_point) * initSize);
        }
        (*listSources)[*numSource].x = x;
        (*listSources)[*numSource].y = y;
        (*listSources)[*numSource].value = value;
        (*grid)[x][y] = value;
        *numSource = *numSource + 1;
        getline(&line, &len, fp);
    }
    free(line);
    return 0;
}

/**
 * Parse the file
 * @param fp file
 * @param grid the grid
 * @param n width
 * @param m length
 * @param listSources list of sources
 * @param numSource number of sources
 * @param terminate
 * @param n_iter
 * @param is_cyclic
 */
void parseFile(FILE *fp, double ***grid, int *n, int *m, source_point **listSources, int *numSource, double *terminate,
               unsigned int *n_iter, int *is_cyclic)
{
    if (fp == NULL)
    {
        exit(1);
    }
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, fp); // get size of the board
    int initSize = INIT_SIZE, iter;
    if (sscanf(line, "%d, %d", n, m) != 2)
    {
        fprintf(stderr, ERROR_SIZE);
        closeAndFree(line, NULL, *n, NULL, fp);
    }
    if (*n < 0 || *m < 0)
    {
        fprintf(stderr, ERROR_SIZE);
        closeAndFree(line, NULL, *n, NULL, fp);
    }
    if (initGrid(grid, *n, *m))
    {
        fprintf(stderr, ERROR_MEMORY);
        closeAndFree(line, NULL, *n, NULL, fp);
    }
    getline(&line, &len, fp);
    *listSources = (source_point *) malloc(sizeof(source_point) * initSize);
    if (*listSources == NULL)
    {
        fprintf(stderr, ERROR_MEMORY);
        closeAndFree(line, grid, *n, *listSources, fp);
    }
    if (strcmp(SEPARATOR, line) == 0)
    {
        if (findSources(line, fp, *n, *m, grid, listSources, numSource, initSize))
        {
            closeAndFree(line, grid, *n, *listSources, fp);
        }
    }
    getline(&line, &len, fp);

    if (sscanf(line, "%lf\n", terminate) != 1)
    {
        fprintf(stderr, ERROR_TERMINATE);
        closeAndFree(line, grid, *n, *listSources, fp);

    }
    getline(&line, &len, fp);
    if (sscanf(line, "%d", &iter) != 1 || iter < 0)
    {
        fprintf(stderr, ERROR_N_ITER);
        closeAndFree(line, grid, *n, *listSources, fp);
    }
    *n_iter = (unsigned int) iter;
    getline(&line, &len, fp);
    if (sscanf(line, "%d", is_cyclic) != 1)
    {
        fprintf(stderr, ERROR_CYCLIC);
        closeAndFree(line, grid, *n, *listSources, fp);
    };
    free(line);
}

/**
 * print the delta value and the grid
 * @param grid the grid
 * @param delta the delta
 * @param n width
 * @param m length
 */
void printGrid(double **grid, double delta, int n, int m)
{
    if (delta < 0)
    {
        printf("%lf\n", -delta);
    }
    else
    {
        printf("%lf\n", delta);
    }
    for (int j = 0; j < n; ++j)
    {
        for (int i = 0; i < m; ++i)
        {
            printf("%.4f,", grid[j][i]);
        }
        printf("\n");
    }
}

/**
 * The main function
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    FILE *fp;
    (void) argc;
    double **grid;
    source_point *listSources;
    int n, m, numSource = 0, is_cyclic;
    double terminate;
    unsigned int n_iter;

    fp = fopen(argv[1], "r");
    parseFile(fp, &grid, &n, &m, &listSources, &numSource, &terminate, &n_iter, &is_cyclic);
    fclose(fp);

    double delta = calculate(heat_eqn, grid, (size_t) n, (size_t) m, listSources, (size_t) numSource, terminate, n_iter,
                             is_cyclic);
    printGrid(grid, delta, n, m);

    while (delta > terminate || -delta > terminate)
    {
        delta = calculate(heat_eqn, grid, (size_t) n, (size_t) m, listSources, (size_t) numSource, terminate, n_iter,
                          is_cyclic);
        printGrid(grid, delta, n, m);

    }
    closeAndFree(NULL, &grid, n, listSources, NULL);
    return 0;
}