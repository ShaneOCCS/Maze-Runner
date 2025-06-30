/* Assignment 1: Maze Runner.
   Student Name: Shane O'Connell
   Student Number: #041144343
   Professor Name: Frank Emanuel
   Course Name: CST-8234

   File Description: This file hold all main utility functions of the maze runner game
   as prototype functions to be used in mazegen.c and mazeplay.c.
*/

/* Prototype functions to be used later on. */
double checkMaze(char** maze, int rows, int cols);
int* findValidPath(char *row, size_t rowLength, char target, size_t* buff);
int generateRandomNumber(int min, int max);
void mazePath(char *row, char *previousRow, int columns, int index, int lastIndex);
void printMaze(char **maze, int rows, int columns);
char **generateMaze(int rows, int columns);
void getMazeStart(char** maze, int cols, int* playerX, int* playerY);
char getCharAt(char** maze, int posX, int posY);
void setCharAt(char** maze, int posX, int posY, char character);
void playerMove(char** maze, int posX, int posY, int* playerPosX, int* playerPosY, int* moveCount);
void freeMemory(char** maze, int rows);
