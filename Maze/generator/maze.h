#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mergeGroup(int **maze, int row, int N, int from, int to);
void printMaze(char *filename, int N, int M, int **rightWall, int **bottomWall);
void generateMaze(int **maze, int **rightWall, int **bottomWall, int *groupList, int N, int M);
void initMaze(int **maze, int **rightWall, int **bottomWall, int N, int M);
void freeMaze(int **maze, int **rightWall, int **bottomWall, int *groupList, int M);
