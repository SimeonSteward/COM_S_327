#include "dungeonMaker.h"

char map[HEIGHT][WIDTH];//y,x
int main(int argc, const char *argv[]) {
    srand(time(NULL));
    int roomCount = 0;
    initializeMap();
    printMap();
    //createRoom();
    while (roomCount < 6) {
        for (int i = 0; i < 40; i++) {
            printf("%d", i);
            if (createRoom()) {
                roomCount++;
            }
        }
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
    int x1 = rand() % WIDTH;
    int y1 = rand() % HEIGHT;
      int xMag = rand()%10+4;
    int yMag = rand()%10+3;
//    int xMag = 4;
//    int yMag = 4;
    printf("x1: %d,y1: %d, xMag: %d, yMag: %d \n", x1, y1, xMag, yMag);
    for (int i = -1; i <= yMag; i++) {
        for (int j = -1; j <= xMag; j++) {
            if (i + y1 >= HEIGHT || i + y1 <= 0 || j + x1 >= WIDTH || j + x1 <= 0 || !(map[i + y1][j + x1] == ' ')) {
                printf("%d%d%d%d%d", i + y1 >= HEIGHT, i + y1 <= 0, j + x1 >= WIDTH, j + x1 <= 0,
                       !(map[i + y1][j + x1] == ' '));
                printf("map:\'%c\'", map[i + y1][i + x1]);
                printf("Failed i:%d,j%d ", i, j);
                return false;
            }
        }
    }
    for (int i = 0; i < yMag; i++) {
        for (int j = 0; j < xMag; j++) {
            map[i + y1][j + x1] = '.';
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
