#include "dungeonMaker.h"

typedef struct room{
int x1;
int y1;
int xMag;
int yMag;
}room;

struct pc{
  int8_t x,y;
} pc;


room rooms[MAXROOMS];
int numRooms;
uint8_t hardness[HEIGHT][WIDTH];
char map[HEIGHT][WIDTH];//y,x
int main(int argc, const char *argv[]) {
    srand(time(NULL));
    initializeMap();
//    printMap();
    //createRoom();
    numRooms=0;

    for (int i = 0; i < MAXROOMS; i++) {
//            printf("%d", i);
        createRoom();
    }

//    printMap();

//    for(int i = 0;i<numRooms;i++){
//        printRoomData(i);
//    }
    for(int i = 0; i<numRooms-1;i++){
        digTunnel(i,i+1);
    }
    addStairs();
    printMap();
}

bool loadMap(char * path){

  FILE *f = fopen(path, "r");

  char semantic[13];
  semantic[12] = '\0';
  fread(semantic, 1,12,);
  int version;
  fread(&version,4,1,f);
  version = be32toh(version);
  int size;
  fread(&size, 4, 1, f);
  size = be32toh(size);
  fread(&pc.x,1,1,f);
  fread(&pc.y,1,1,f);

}

bool saveMap(char * path){
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
            if (i + room1.y1 >= HEIGHT || i + room1.y1 <= 0 || j + room1.x1 >= WIDTH || j + room1.x1 <= 0 ||
                map[i + room1.y1][j + room1.x1] != ' ') {
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
            hardness[i + room1.y1][j + room1.x1] = 0;
        }
    }

    rooms[numRooms++] = room1;
    return true;

}

void printMap() {
  if(DEBUG){
    for (int i = -1; i <= HEIGHT; i++) {
      printf("\n");

      if((i==-1)|(i==HEIGHT)){
        printf(" ");
        for (int j = 0; j < WIDTH; j++) {
          printf("%d", j%10);
        }
      }else {
        for (int j = -1; j <= WIDTH; j++) {
          if((j==-1)|(j==WIDTH)){
            printf("%d",i%10);
          }else {
            printf("%c", map[i][j]);
          }
        }

      }
    }
  }else{
    for(int i = 0; i<HEIGHT;i++){
      printf("\n");
      for(int j = 0; j<WIDTH;j++){
        printf("%c", map[i][j]);
      }
    }
  }
       printf("\n");
    fflush(stdout);
}
void printRoomData(int roomNumber){
    room room1 = rooms[roomNumber];
    printf("Room %d has an x: %d y: %d xMag: %d yMag: %d\n",roomNumber,room1.x1,room1.y1,room1.xMag,room1.yMag);
}
bool digTunnel(int first, int second){
    int curX = rooms[first].x1;
    int curY = rooms[first].y1;
    int destX = rooms[second].x1;
    int destY = rooms[second].y1;
    int xDelta = destX-curX;
    int yDelta = destY-curY;
//    map[curY][curX] = first+'0';
//    map[destY][destX]= second+'0';
    while(xDelta!=0||yDelta!=0){
//        printMap();

      if(rand()%(abs(xDelta)+abs(yDelta))<abs(xDelta)){
          curX += xDelta/abs(xDelta);
      }else{
          curY += yDelta/abs(yDelta);
      }
      if(map[curY][curX]==' '){
          map[curY][curX]='#';
          hardness[curY][curX]=0;
      }
      xDelta = destX-curX;
      yDelta = destY-curY;

    }
    return true;

}

bool addStairs(){
    bool needUpStairs = true;
    bool needDownStairs = true;
    while(needUpStairs||needDownStairs){
        int x = rand()%WIDTH;
        int y = rand()%HEIGHT;
        if(map[y][x]=='.'){
            map[y][x]='<';
            needUpStairs = false;
        }
        x = rand()%WIDTH;
        y = rand()%HEIGHT;
        if(map[y][x]=='.'){
            map[y][x]='>';
            needDownStairs = false;
        }
    }
    return true;
}
