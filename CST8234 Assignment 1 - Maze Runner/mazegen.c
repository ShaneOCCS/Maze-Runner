/* Assignment 1: Maze Runner.
   Student Name: Shane O'Connell
   Student Number: #041144343
   Professor Name: Frank Emanuel
   Course Name: CST-8234

   File Description: This file contains all maze generation functionality 
   for the maze runner program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utility.h"

/* This method frees memory for the maze and the rows of the maze to prevent leaks. */
void freeMemory(char** maze, int rows) {
    int i;
    for (i = 0; i < rows; i++) { /* Loop to free each individual row of the maze. */
                char* row = *((char **)((char *)maze + i * sizeof(char *))); 
                free(row);
            }
            free(maze);
}

/* Checks the % of period characters there are in a maze. */
double checkMaze(char** maze, int rows, int cols) {
    int i;
    int j;
    int count = 0;
    int sizeOfMap = rows * cols;
    double percent = ((double)count / sizeOfMap) * 100.0; /* Calculates the percent for the maze, used later on. */

    /* Loop through the maze. */
    for (i = 0; i < rows; i++) { /* Outer loop iterates through each row pointer in the maze structure. */
        char *row = *((char **)((char *)maze + i * sizeof(char *)));
        for (j = 0; j < cols; j++) { /* Inner loop iterates through and prints each character pointed to by the current row pointer. */
            if (*((char *)((char *)row + sizeof(char) * j)) == '.') { /* Counts the amount of characters. */
                count++;
            }
        }
    }
    return percent;
}

/* findValidPath finds a correct pathway so the player can reach the end of the maze, acts a helper method. */
int* findValidPath(char *row, size_t rowLength, char target, size_t* buff) {
    size_t i;
    int matchCount = 0;
    int* pathVals;
    int currentPathIndex;

    for (i = 0; i < rowLength; i++) { /* Counts the amount of matches. */
        if (target == *((char *)row + sizeof(char) * i)) {
            matchCount++;
        }
    }

    if (buff != NULL) { /* Update with mathcount. */
        *buff = matchCount;
    }

    if (matchCount == 0) { /* If there are no matches. */
        return NULL;
    }

    pathVals = malloc(sizeof(int) * matchCount); /* Allocate memory for path values. */

    if (pathVals == NULL) { /* If malloc fails. */
        printf("%s\n", "Malloc failed for pathVals in findValidPath. Attempting to continue.");
        if (buff != NULL) {
            *buff = 0;
        }
        return NULL;
    }

    currentPathIndex = 0;
    for (i = 0; i < rowLength; i++) { /* Loop to store the target chars. */
        if (target == *((char *)row + sizeof(char) * i)) {
            *((int*)((char*)pathVals + sizeof(int) * currentPathIndex)) = i;
            currentPathIndex++;
        }
    }
    return pathVals;
}

/* Generates a random int. */
int generateRandomNumber(int min, int max) {
    int randomNumber;
    if (min > max) { /* Check to see if the max is actually the max. */
        int temp = min;
        min = max;
        max = temp;
    }
    randomNumber = (rand() % (max - min + 1)) + min; /* Actual generation of random integer. */
    return randomNumber;
}

/* Generates a path in the maze based on the previous rows path. */
void mazePath(char *row, char *previousRow, int columns, int index, int lastIndex) {
    int* dotPathVals;
    size_t dotCount = 0;
    size_t i;

    if (previousRow == NULL) { /* Ensure that the first row can connect to the starting point. */
        int randomStart = generateRandomNumber(1, columns -2);
        *(((char *)row + sizeof(char) * randomStart)) = '.';
        return;
    }

    if (index % 2 == 0 || index == 1) { /* For even rows. */             
        if (index == 1) { 
            int* sPathVals = findValidPath(previousRow, columns, '.', NULL);
            if (sPathVals != NULL) {
                int sColIndex = *(int*)((char*)sPathVals + sizeof(int) * 0);
                *(((char *)row + sizeof(char) * sColIndex)) = '.';
            }
            free(sPathVals);
            return;
        } 

        dotPathVals = findValidPath(previousRow, columns, '.', &dotCount); /* For other even rows, extend pathing. */

        for (i = 0; i < dotCount; i++ ) { 
            if (dotPathVals != NULL && dotCount > 0) { /* Checks if dotpaths is valid. */
                int chosenPrevColIdx = *(int*)((char*)dotPathVals + sizeof(int) * i);
                int rightBound = generateRandomNumber(chosenPrevColIdx, columns - 2); /* Determines right bound.*/
                int leftBound = generateRandomNumber(1, chosenPrevColIdx); /* Determines left bound.*/
                int i;
                for (i = leftBound; i <= rightBound; i++) { /* Set each character in the current row within these bounds to a '.'. */
                    *(((char *)row + sizeof(char) * i)) = '.';
                }
            }
        }
        free(dotPathVals);
    } else {
        size_t buffVal = 0;
        int* validPaths = findValidPath(previousRow, columns, '.', &buffVal); /* Find valid path locations in the previous row. */
        int randNum = generateRandomNumber(0, buffVal -1); 
        int randIndex = *(int*)((char*)validPaths + sizeof(int) * (randNum));
        *(((char *)row + sizeof(char) * randIndex)) = '.';  /* Sets the chosen path spot to a '.'. */
        free(validPaths);
    }

    if (index == lastIndex -1) { /* Handles the absolute last row. */
        size_t buffVal = 0;
        int* validPaths = findValidPath(previousRow, columns, '.', &buffVal);
        int i;
        for (i = 0; i < columns; i++) { /* Loops through the last row to place an E so the user can exit the program. */
            if (i == *validPaths) { 
                *(((char *)row + sizeof(char) * i)) = 'E'; 
            } else {
                *(((char *)row + sizeof(char) * i)) = '#';
            }
        }
    }
}

/* printMaze will print the maze to the console. */
void printMaze(char **maze, int rows, int columns) {
    int i;
    int j;
    for (i = 0; i < rows; i++) { /* Outer loop iterates through each row pointer in the maze structure. */
        char *row = *((char **)((char *)maze + i * sizeof(char *))); /* Dereferences the pointer to the 'i'-th row pointer within the maze structure. */
        for (j = 0; j < columns; j++) { /* Inner loop iterates through and prints each character pointed to by the current row pointer. */
            printf("%c", *(((char *)row + sizeof(char) * j)));
        }
        printf("\n"); /* Formats the maze properly. */
    }
}

/* Generates a dynamic maze and will not ever generate if its more than 25% '.' characters. */
char **generateMaze(int rows, int columns) { 
    char **maze = malloc(sizeof(char *) * rows); /* Initalizes the maze. */
    int i;
    int j;

    if (!maze) { /* Check if malloc failed for the maze. */
        printf("%s", "Maze Malloc Failed");
    }

    for (i = 0; i < rows; i++) { /* Allocate memory for each row of the maze. */
        char *row = malloc(columns * sizeof(char));

        if (!row) { /* Checks if the allocation failed. */
            printf("%s", "Maze Malloc (row) Failed");
        }

        *((char **)((char *)maze + i * sizeof(char *))) = row; /* Stores the pointer to the newly allocated character segment into the maze. */
        for (j = 0; j < columns; j++) { /* Make the walls for the maze. */
            *(((char *)row + sizeof(char) * j)) = '#';
        }

        if (i == 0) { /* Check to see if it is the first row of the maze. */
            mazePath(row, NULL, columns, i, rows);
        } else { /* Check if the previous row is null. */
            char *previousRow = *((char **)((char *)maze + (i - 1) * sizeof(char *)));
            if (previousRow == NULL) {
                continue;
            }
            mazePath(row, previousRow, columns, i, rows);
        }
    }

    while (checkMaze(maze, rows, columns) > 25.0) { /* Regenerate the maze if there is too many '.' characters. */
        freeMemory(maze, rows);
        maze = generateMaze(rows, columns); /* Generates a new maze replacing previous maze. */
    }
    return maze;
}
