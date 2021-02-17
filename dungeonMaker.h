#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <endian.h>

#define WIDTH 80
#define HEIGHT 21
#define MAXROOMS 6
#define DEBUG false


typedef struct room{
  int8_t x1;
  int8_t y1;
  int8_t xMag;
  int8_t yMag;
}room;

bool initializeMap();
bool createRoom();
bool buildRoom(room room1);
void printMap();
void printRoomData(int roomNumber);
bool digTunnel(int first, int second);
bool addStairs();

