
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "assignment0.h"

#define SIZE 23
int main(int argc, const char* argv[] )
{
  printf("hello");
  int fps = 60;
  int parameter = 1;
  int table[SIZE][SIZE];
  int i,j;

  for(i=0;i<SIZE;i++){
    for(j=0;j<SIZE;j++){
      table[j][i] = 0;
    }
  }
  printf("21");
  if(argc>1&&!strcmp(argv[1],"--fps")){
    fps = atoi(argv[2]);
    parameter = 3;
  }
  printf("25");
  printf("Parameter: %d \nArgc: %d\n",parameter,argc);

  for(;parameter+2<argc;parameter+=3){
    table[atoi(argv[parameter+1])][atoi(argv[parameter])] = atoi(argv[parameter+2]);

  }
  
  printf("30");
   while(true){
    displayTable(table);
     usleep(1000000/fps);
    //Sleep(1000/fps);
    table[SIZE/2][SIZE/2]++;
    bool spilled = true;
    while(spilled){
      spilled = false;
      for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
          if(table[j][i]>=8){
            spill(table, j, i);
            spilled = true;
          }
        }
      }
    }
  }
}

void displayTable(int table[SIZE][SIZE])
{
  printf("\n");
  int i,j;
  for(i=0;i<SIZE;i++){
    for(j=0;j<SIZE;j++){
      if(table[j][i]>=0){
        printf("%d ", table[j][i]);
      }else{
        printf("# ");
      }
    }
    printf("\n");
  }
}

void spill(int table[SIZE][SIZE],int x,int y){
  table[x][y]-=8;
  int i,j;
  for(i=-1;i<=1;i++){
    for(j=-1;j<=1;j++){
      if(i+y>=0&&j+x>=0&&i+y<SIZE&&j+x<SIZE&&(!(i==0&&j==0))&&table[j+x][i+y]>=0){
          table[j+x][i+y]++;
        }
    }
  }
}
