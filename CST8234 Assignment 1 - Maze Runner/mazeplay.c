/* Assignment 1: Maze Runner.
   Student Name: Shane O'Connell
   Student Number: #041144343
   Professor Name: Frank Emanuel
   Course Name: CST-8234

   File Description: This file holds the main method and all 
   player functionality in the maze runner program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utility.h"

/* This function will find the starting position of the program, used a helper method so we know where the maze begins. */
void getMazeStart(char** maze, int cols, int* playerX, int* playerY) {
    char* firstRow = *((char **)((char *)maze + 0 * sizeof(char *))); /* Get the pointer to the first row of the maze. */
    size_t size = 0;
    int* position = findValidPath(firstRow, cols, '.', &size);
    *playerY = 0; /* Set player's starting Y-coordinate to the first row. */
    *playerX = *position; /* Set player's starting X-coordinate to the found position. */
}

/* Retrieves the character at a specific position in the maze. */
char getCharAt(char** maze, int posX, int posY) {
    char *row = *((char **)((char *)maze + posY * sizeof(char *))); /* Get the pointer to the specified row. */
    return *(((char *)row + sizeof(char) * posX)); /* Return specified character. */
}

/* Sets a character at a specific position in the maze. */
void setCharAt(char** maze, int posX, int posY, char character) {
    char *row = *((char **)((char *)maze + posY * sizeof(char *))); /* Get the pointer to the specified row. */
    *(((char *)row + sizeof(char) * posX)) = character; /* Set the character at the specified column in that row. */
}

/* Handles player movement within the maze, updating their position and checking for walls/exit. */
void playerMove(char** maze, int posX, int posY, int* playerPosX, int* playerPosY, int* moveCount) {
    if (posY < 0) { /* Pervent going outside the maze so user doesn't seg fault the program. */
        printf("%s\n", "You cannot move back on the first move, nice try :D ");
        return;
    }

    if (getCharAt(maze, posX, posY) == 'E') { /* Victory message. */
        printf("%s %d %s\n", "Congratulations you beat the maze in:", *moveCount , "moves!");
        printf("%s\n", "Would you like to play again? (Y/N): ");
        return;
    }

    if (getCharAt(maze, posX, posY) != '.' && getCharAt(maze, posX, posY) != '*') { /* Checks if it is a wall. */
        printf("%s\n", "You walked into a wall! ");
        return;
    }

    setCharAt(maze, *playerPosX, *playerPosY, '*'); /* Sets position the user left to a '*' character. */
    *playerPosX = posX;
    *playerPosY = posY;
    (*moveCount)++;
    setCharAt(maze, posX, posY, 'S');
}

/* Main function handles player inputs and executes all player functionality. */
int main() {
    int moveCount = 0;
    int playerX = 0;
    int playerY = 0;
    int columns;
    int rows;
    char **maze;
    int i;

    srand((unsigned int)time(NULL)); /* Seeds the random generator. */

    columns = generateRandomNumber(10, 20); /* Sets the sizing of the maze cols. */
    rows = generateRandomNumber(10, 20); /* Sets the sizing of the maze rows. */

    maze = generateMaze(rows, columns); 
    getMazeStart(maze, columns, &playerX, &playerY);
    
    printf("%s\n", "Welcome to Maze Runner!");
    printf("%s\n", "Maze Generated. Reach the exit (E).\n");
    printf("Use W/A/S/D to move. Press Q to exit: \n"); 

    while (1) { /* Menu for player input. */
        char move = ' ';
        setCharAt(maze, playerX, playerY, 'S');
        printMaze(maze, rows, columns); /* Prints the maze each time there is user input. */

        scanf(" %c", &move);

        switch (move) { 
        case 'W': /* Move forward case. */
        case 'w':
            printf("Move (W/A/D/S): \n"); 
            playerMove(maze, playerX, playerY + 1, &playerX, &playerY, &moveCount);
            break;

        case 'A': /* Move left case. */
        case 'a':
            printf("Move (W/A/D/S): \n"); 
            playerMove(maze, playerX -1, playerY, &playerX, &playerY, &moveCount);
            break;

        case 'S': /* Move backwards case. */
        case 's':
            printf("Move (W/A/D/S): \n"); 
            playerMove(maze, playerX, playerY - 1, &playerX, &playerY, &moveCount);
            break;

        case 'D': /* Move right case. */
        case 'd':
            printf("Move (W/A/D/S): \n"); 
            playerMove(maze, playerX + 1, playerY, &playerX, &playerY, &moveCount);
            break;

        case 'Q': /* Exit the program case. */
        case 'q':
            printf("%s\n", "Exiting the program.");
            printf("%s\n", "Program made by Shane O'Connell (041144343).");
               for (i = 0; i < rows; i++) { /* Loop to free each individual row of the maze. */
                char* row = *((char **)((char *)maze + i * sizeof(char *))); 
                free(row);
            }
            free(maze);
            exit(1);

        case 'Y': /* If the user would like to play again after victory case. */
        case 'y':
            moveCount = 0;
              
            freeMemory(maze, rows);

            rows = generateRandomNumber(10, 20);
            columns = generateRandomNumber(10, 20);
            maze = generateMaze(rows, columns);
            getMazeStart(maze, columns, &playerX, &playerY);
            setCharAt(maze, playerX, playerY, 'S');
            break;

        case 'N': /* If the user would like to exit after victory case. */
        case 'n':
            printf("\n");
            printf("%s\n", "Thanks for playing.");
            printf("%s\n", "Now exiting the program.");
            printf("%s\n", "Program made by Shane O'Connell (041144343).");
            
            freeMemory(maze, rows);
            
            exit(1);

        default:
            printf("%s\n", "Enter W A S D or Q to use the maze runner program.");
            break;
        }
    }
    return 0; /* Program completion. */
}
