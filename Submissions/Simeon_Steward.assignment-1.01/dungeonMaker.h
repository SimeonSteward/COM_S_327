#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 21
#define MAXROOMS 6
#define DEBUG false

bool initializeMap();
bool createRoom();
void printMap();
void printRoomData(int roomNumber);
bool digTunnel(int first, int second);
bool addStairs();
