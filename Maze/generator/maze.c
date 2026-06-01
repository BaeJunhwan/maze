#include "maze.h"


int main(){
    int N, M;
    int **maze;
    int **rightWall;
    int **bottomWall;
    int *groupList;

    srand(time(NULL));

    scanf("%d", &N); //너비
    scanf("%d", &M); //높이

    maze = malloc(sizeof(int *) * M);
    rightWall = malloc(sizeof(int *) * M);
    bottomWall = malloc(sizeof(int *) * M);
    groupList = malloc(sizeof(int) * N);

    for(int i = 0; i < M; i++) {
        maze[i] = malloc(sizeof(int) * N);
        rightWall[i] = malloc(sizeof(int) * N);
        bottomWall[i] = malloc(sizeof(int) * N);
    }

    initMaze(maze, rightWall, bottomWall, N, M);

    generateMaze(maze, rightWall, bottomWall, groupList, N, M);

    printMaze("maze.maz", N, M, rightWall, bottomWall);

    freeMaze(maze, rightWall, bottomWall, groupList, M);

    return 0;
}

void initMaze(int **maze, int **rightWall, int **bottomWall, int N, int M){
    for(int i=0; i<M; i++) {
        for(int j=0; j<N; j++) {
            maze[i][j]=0;
            rightWall[i][j]=1;
            bottomWall[i][j]=1;
        }
    }

    for(int j=0; j<N; j++) {
        maze[0][j]=j+1;
    }
}

void generateMaze(int **maze, int **rightWall, int **bottomWall, int *groupList, int N, int M) {
    int groupCount;
    int elementCount;
    int nextGroup=N+1;
    int target, idx, chosen;

    for(int i=0; i<M-1; i++) {
        groupCount=0;

        for(int j=0; j<N-1; j++) {
            if(maze[i][j]!=maze[i][j+1]) {
                rightWall[i][j]=rand()%2;

                if(rightWall[i][j]==0) {
                    mergeGroup(maze, i, N, maze[i][j+1], maze[i][j]);
                }
            }
        }
        groupCount=0;
        
        for(int j=0; j<N; j++) {
            int exist=0;

            for(int k=0; k<groupCount; k++) {
                if(groupList[k]==maze[i][j]) {
                    exist=1;
                    break;
                }
            }

            if(!exist) {
                groupList[groupCount]=maze[i][j];
                groupCount++;
            }
        }

        for(int k=0; k<groupCount; k++) {
            elementCount=0;

            for(int j=0; j<N; j++) {
                if(maze[i][j]==groupList[k]) {
                    elementCount++;
                }
            }

            target=rand()%elementCount;
            idx=0;
            chosen=-1;

            for(int j=0; j<N; j++) {
                if(maze[i][j]==groupList[k]) {
                    if(target==idx) {
                        bottomWall[i][j]=0;
                        chosen=j;
                        break;
                    }
                    idx++;
                }
            }

            for(int j=0; j<N; j++) {
                if(maze[i][j]==groupList[k] && j!=chosen) {
                    bottomWall[i][j]=rand()%2;
                }
            }
        }

        for(int j=0; j<N; j++) {
            if(bottomWall[i][j]==1) {
                maze[i+1][j]=nextGroup++;
            }
            else {
                maze[i+1][j]=maze[i][j];
            }
        }
    }

    for(int j=0; j<N-1; j++) {
        if(maze[M-1][j]!=maze[M-1][j+1]) {
            rightWall[M-1][j]=0;
            mergeGroup(maze, M-1, N, maze[M-1][j+1], maze[M-1][j]);
        }
    }
}

void freeMaze(int **maze, int **rightWall, int **bottomWall, int *groupList, int M) {
    for(int i=0; i<M; i++) {
        free(maze[i]);
        free(rightWall[i]);
        free(bottomWall[i]);
    }

    free(maze);
    free(rightWall);
    free(bottomWall);
    free(groupList);
}

void mergeGroup(int **maze, int row, int N, int from, int to) {
    for(int j=0; j<N; j++) {
        if(maze[row][j]==from) {
            maze[row][j]=to;
        }
    }
}

void printMaze(char *filename, int N, int M, int **rightWall, int **bottomWall) {
    FILE *fp = fopen(filename, "w");

    if(fp==NULL) {
        printf("file open error!\n");
        return;
    }

    for(int j=0; j<N; j++) {
        fprintf(fp,"+-");
    }
    fprintf(fp, "+\n");

    for(int i=0; i<M; i++) {
        fprintf(fp, "|");
        for(int j=0; j<N; j++) {
            fprintf(fp, " ");
            if(rightWall[i][j]==1) {
                fprintf(fp, "|");
            }
            else {
                fprintf(fp, " ");
            }
        }

        fprintf(fp, "\n");
        fprintf(fp, "+");

        for(int j=0; j<N; j++) {
            if(bottomWall[i][j]==1) {
                fprintf(fp, "-+");
            }
            else {
                fprintf(fp, " +");
            }
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}