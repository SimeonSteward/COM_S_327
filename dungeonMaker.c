#include "dungeonMaker.h"

typedef struct room{
int8_t x1;
int8_t y1;
int8_t xMag;
int8_t yMag;
}room;

typedef struct upStair{
  int8_t x,y;
}upStair;



typedef struct downStair{
  int8_t x,y;
}downStair;

struct pc{
  int8_t x,y;
} pc;


room* rooms;

int16_t numRooms;
u_int8_t hardness[HEIGHT][WIDTH];
char map[HEIGHT][WIDTH];//y,x


int main(int argc, const char *argv[]) {
  bool saveMode;
  bool loadMode;
  if(argc>1){
    saveMode = !strcmp(argv[1],"--save");
    loadMode = !strcmp(argv[1],"--load");
  }
  if(argc>2){
    saveMode = saveMode || !strcmp(argv[2],"--save");
    loadMode = loadMode || !strcmp(argv[2],"--load");
  }
    srand(time(NULL));
    initializeMap();
    numRooms=0;

    rooms =(room*) malloc(4*MAXROOMS);
    for (int i = 0; i < MAXROOMS; i++) {
        createRoom();
    }

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
  fread(semantic, 1,12,f);
  int version;
  fread(&version,4,1,f);
  version = be32toh(version);
  int size;
  fread(&size, 4, 1, f);
  size = be32toh(size);
  fread(&pc.x,1,1,f);
  fread(&pc.y,1,1,f);
  fread(hardness,1,1680,f);
  fread(&numRooms,2,1,f);
  numRooms = be16toh(numRooms);
  rooms = (room*) malloc(4*numRooms);
  for(int i = 0;i<numRooms;i++){
    fread(&(rooms[i].x1),1,1,f);
    fread(&(rooms[i].y1),1,1,f);
    fread(&(rooms[i].xMag),1,1,f);
    fread(&(rooms[i].yMag),1,1,f);
  }
  int16_t numUp;
  fread(&numUp,2,1,f);
  numUp = be16toh(numUp);
  
  upStair * upStairs = (upStair*)malloc(2*numUp);
  for(int i = 0; i<numUp;i++){
    fread(&(upStairs[i].x),1,1,f);
    fread(&(upStairs[i].y),1,1,f);
  }

  int16_t numDown;
  fread(&numDown,2,1,f);
  numDown = be16toh(numDown);
  
  downStair * downStairs = (downStair*)malloc(2*numDown);
  for(int i = 0; i<numDown;i++){
    fread(&(downStairs[i].x),1,1,f);
    fread(&(downStairs[i].y),1,1,f);
  }

  return true;

}

bool saveMap(char * path){
  return true;
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
    for (int i = -1; i <= room1.yMag; i++) {
        for (int j = -1; j <= room1.xMag; j++) {
            if (i + room1.y1 >= HEIGHT || i + room1.y1 <= 0 || j + room1.x1 >= WIDTH || j + room1.x1 <= 0 ||
                map[i + room1.y1][j + room1.x1] != ' ') {
                createRoom();
                return false;
            }
        }
    }
    buildRoom(room1);
       return true;

}

bool buildRoom(room room1){
  for (int i = 0; i < room1.yMag; i++) {
    for (int j = 0; j < room1.xMag; j++) {
      map[i + room1.y1][j + room1.x1] = '.';
      hardness[i + room1.y1][j + room1.x1] = 0;
    }
  }

  rooms[numRooms++] = room1;
 
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
