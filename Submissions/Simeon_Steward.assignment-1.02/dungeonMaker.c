#include "dungeonMaker.h"

typedef struct upStair{
  int8_t x,y;
}upStair;



typedef struct downStair{
  int8_t x,y;
}downStair;

struct pc{
  int8_t x,y;
} pc;

int16_t numUp = 0;
int16_t numDown = 0;

room* rooms;
upStair* upStairs;
downStair* downStairs;
int16_t numRooms;
u_int8_t hardness[HEIGHT][WIDTH];
char map[HEIGHT][WIDTH];//y,x


int main(int argc, const char *argv[]) {
  srand(time(NULL));
  char * home = getenv("HOME");
  char * game_dir = ".rlg327";
  const char * save_file = "dungeon";
  bool saveMode = false;
  bool loadMode = false;
  for(int i = 1; i<argc;i++){
    saveMode = saveMode||!strcmp(argv[i],"--save");
    loadMode = loadMode||!strcmp(argv[i],"--load");
    if((argc>i+1)&&(!strcmp(argv[i],"--load")||!strcmp(argv[i],"--save"))){
        save_file = argv[i+1];
    }
  }




  char * path = malloc(strlen(home)+strlen(game_dir)+strlen(save_file)+2+1);
  sprintf(path,"%s/%s/%s",home,game_dir,save_file);
  initializeMap();

  if(loadMode){
    loadMap(path);
  }else{
        numRooms=0;

    rooms =(room*) malloc(4*MAXROOMS);
    for (int i = 0; i < MAXROOMS; i++) {
        createRoom();
    }

    for(int i = 0; i<numRooms-1;i++){
        digTunnel(i,i+1);
    }
    addPlayerCharacter();

    addStairs();

  }
  printMap();
  if(saveMode){
    saveMap(path);
  }
}

bool loadMap(char * path){

  FILE *f = fopen(path, "r");
  if(f == NULL){
    printf("FILE NOT FOUND\n");
    exit(1);

  }


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
    fread(&(rooms[i].x1),sizeof(rooms[0].x1),1,f);
    fread(&(rooms[i].y1),sizeof(rooms[0].y1),1,f);
    fread(&(rooms[i].xMag),sizeof(rooms[0].xMag),1,f);
    fread(&(rooms[i].yMag),sizeof(rooms[0].yMag),1,f);
  }
  fread(&numUp,2,1,f);
  numUp = be16toh(numUp);
  
  upStairs = (upStair*)malloc(sizeof(upStair)*numUp);
  for(int i = 0; i<numUp;i++){
    fread(&(upStairs[i].x),sizeof(upStairs[0].x),1,f);
    fread(&(upStairs[i].y),sizeof(upStairs[0].y),1,f);
  }

  fread(&numDown,2,1,f);
  numDown = be16toh(numDown);
  
  downStairs = (downStair*)malloc(2*numDown);
  for(int i = 0; i<numDown;i++){
    fread(&(downStairs[i].x),1,1,f);
    fread(&(downStairs[i].y),1,1,f);
  }
  for(int i = 0;i<HEIGHT;i++){
    for(int j = 0;j<WIDTH;j++){
      if(hardness[i][j]==0){
        map[i][j] = '#';
      }
    }
  }
  for(int i = 0;i<numRooms;i++){
    buildRoom(rooms[i]);
  }
  for(int i = 0;i<numUp;i++){
    map[upStairs[i].y][upStairs[i].x] = '<';
  }
  for(int i = 0; i<numDown;i++){
    map[downStairs[i].y][downStairs[i].x] = '>';
  }

  return true;

}

bool saveMap(char * path){
  FILE *f = fopen(path, "w");
  if(f == NULL){
    printf("FILE NOT FOUND\n");
    exit(1);
  }
  char semantic[13] = "RLG327-S2021";
  fwrite(semantic,1,12,f);
  u_int32_t  version = 0;
  version = htobe32(version);
  fwrite(&version,4,1,f);
  u_int32_t size = 1708+numRooms*4+numUp*2+numDown*2;
  size = htobe32(size);
  fwrite(&size,4,1,f);
  fwrite(&pc.x,1,1,f);
  fwrite(&pc.y,1,1,f);
  fwrite(hardness,1,1680,f);
  int16_t numRoomsTemp = htobe16(numRooms);
  fwrite(&numRoomsTemp,2,1,f);
  for(int i = 0;i<numRooms;i++){
    fwrite(&(rooms[i].x1),1,1,f);
    fwrite(&(rooms[i].y1),1,1,f);
    fwrite(&(rooms[i].xMag),1,1,f);
    fwrite(&(rooms[i].yMag),1,1,f);
  }
  int16_t numUpTemp = htobe16(numUp);
  fwrite(&numUpTemp,sizeof(numUpTemp),1,f);
  for(int i = 0;i<numUp;i++){
    fwrite(&(upStairs[i].x),sizeof(upStairs[0].x),1,f);
    fwrite(&(upStairs[i].y),sizeof(upStairs[0].y),1,f);
  }
  numDown = htobe16(numDown);
  fwrite(&numDown,2,1,f);
  numDown = be16toh(numDown);
  for(int i = 0;i<numDown;i++){
    fwrite(&(downStairs[i].x),1,1,f);
    fwrite(&(downStairs[i].y),1,1,f);
  }

  

  fclose(f);

  return true;

}
bool initializeMap() {
    int i,j;
    for (i=0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            map[i][j] = ' ';
            if(i==0||j==0||i==HEIGHT-1||j==WIDTH-1){
              hardness[i][j] = 255;
            }else{
              hardness[i][j] = 2;
            }

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
    rooms[numRooms++] = room1;
       return true;

}

bool buildRoom(room room1){
  for (int i = 0; i < room1.yMag; i++) {
    for (int j = 0; j < room1.xMag; j++) {
      map[i + room1.y1][j + room1.x1] = '.';
      hardness[i + room1.y1][j + room1.x1] = 0;
    }
  }

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
              if(pc.x==j&&pc.y==i){
                  printf("@");
              }else{
                  printf("%c", map[i][j]);
              }
          }
        }

      }
    }
  }else{
    for(int i = 0; i<HEIGHT;i++){
      printf("\n");
      for(int j = 0; j<WIDTH;j++){
          if(pc.x==j&&pc.y==i){
              printf("@");
          }else{
        printf("%c", map[i][j]);
          }

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
    upStairs = (upStair *)malloc(2);
    downStairs =(downStair *)malloc(2);
    while(needUpStairs){
        int x = rand()%WIDTH;
        int y = rand()%HEIGHT;
        if(map[y][x]=='.'){
            map[y][x]='<';
            numUp++;
            upStair newUpStair;
            newUpStair.x = x;
            newUpStair.y = y;
            upStairs[0] = newUpStair;
            needUpStairs = false;
        }
    }
    while(needDownStairs){
        int x = rand()%WIDTH;
        int y = rand()%HEIGHT;
        if(map[y][x]=='.'){
            map[y][x]='>';
            numDown++;
            downStair  newDownStair;
            newDownStair.x=x;
            newDownStair.y=y;
            downStairs[0] = newDownStair;
            needDownStairs = false;
        }
    }
    return true;
}
int addPlayerCharacter(){
    bool needPlayer = true;
    while(needPlayer){
        int8_t x = rand()%WIDTH;
        int8_t y = rand()%HEIGHT;
        if(hardness[y][x]==0){
            pc.x=x;
            pc.y=y;
            needPlayer = false;
            return true;
        }
    }
}
