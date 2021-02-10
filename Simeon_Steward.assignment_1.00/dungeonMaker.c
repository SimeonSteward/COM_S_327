#include "dungeonMaker.h"

typedef struct room{
int x1;
int y1;
int xMag;
int yMag;
}room;

room rooms[MAXROOMS];
int numRooms;

char map[HEIGHT][WIDTH];//y,x
int main(int argc, const char *argv[]) {
    srand(time(NULL));
    initializeMap();
    printMap();
    //createRoom();
    numRooms=0;

    for (int i = 0; i < 6; i++) {
//            printf("%d", i);
        createRoom();
    }

    printMap();
}

bool initializeMap() {
    int i,j;
    for (i=0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            map[i][j] = ' ';
       }
    }
    return true;
}

bool createRoom() {
    room room1;
    room1.x1 = rand() % WIDTH;
    room1.y1 = rand() % HEIGHT;
    room1.xMag = rand()%10+4;
    room1.yMag = rand()%10+3;
//    int xMag = 4;
//    int yMag = 4;
//    printf("x1: %d,y1: %d, xMag: %d, yMag: %d \n", x1, y1, xMag, yMag);
    for (int i = -1; i <= room1.yMag; i++) {
        for (int j = -1; j <= room1.xMag; j++) {
            if (i + room1.y1 >= HEIGHT || i + room1.y1 <= 0 || j + room1.x1 >= WIDTH || j + room1.x1 <= 0 || !(map[i + room1.y1][j + room1.x1] == ' ')) {
//                printf("%d%d%d%d%d", i + y1 >= HEIGHT, i + y1 <= 0, j + x1 >= WIDTH, j + x1 <= 0,
//                       !(map[i + y1][j + x1] == ' '));
//                printf("map:\'%c\'", map[i + y1][i + x1]);
//                printf("Failed i:%d,j%d ", i, j);
                createRoom();
                return false;
            }
        }
    }
    for (int i = 0; i < room1.yMag; i++) {
        for (int j = 0; j < room1.xMag; j++) {
            map[i + room1.y1][j + room1.x1] = '.';
        }
    }

    return true;

}

void printMap() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}
