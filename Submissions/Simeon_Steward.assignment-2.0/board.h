#ifndef BOARD_H
# define BOARD_H

#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>

#include "dims.h"

#define MAP_Y 21
#define MAP_X 80

class board {
public:
  board(int num_bombs) : cursor{MAP_X/2,MAP_Y/2}, hidden{0}, visable{0}, started(false),
                         num_bombs(num_bombs),num_safe(0){
    // int y,x;
    // for(y = 0; y<MAP_Y;y++){
    //   for(x = 0;x<MAP_X;x++){
    //     visable[y][x] = 254;
    //   }
    // }
    reset();
  }

  //NOTE
  //255: bomb
  //254: hidden
  //244: flag


  pair_t cursor;
  uint8_t hidden[MAP_Y][MAP_X];
  uint8_t visable[MAP_Y][MAP_X];
  bool started;
  uint16_t num_bombs;
  uint16_t num_safe;
  void reset();
  void populate();
  void set_proximity();
  void print_hidden();
  void print_visable();
  int guess();
  void flag();

};

#endif
