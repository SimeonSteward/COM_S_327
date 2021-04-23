
#include "board.h"
#include "io.h"

#include <stack>
#include <cstring>

// board::board(){
//   for(int y = 0;y<MAP_Y;y++){
//     for(int x = 0;x<MAP_X;x++){
//       hidden[y][x] = 0;
//       visable[y][x] = 0;
//     }
//   }
// }

void board::reset(){
  int y,x;
  for(y = 0; y<MAP_Y;y++){
    for(x = 0;x<MAP_X;x++){
      hidden[y][x] = 0;
      visable[y][x] = 254;
    }
  }
  
}

void board::populate(){
  for(int i = 0; i < num_bombs;i++){

    bool b = false;

    while (!b){
      int y = rand()%MAP_Y;
      int x = rand()%MAP_X;
      if(!hidden[y][x]){
        hidden[y][x] = 255;
        b = true;
      }
    }

  }
}

void board::set_proximity(){
  for(int y = 0;y<MAP_Y;y++){
    for(int x = 0;x<MAP_X;x++){
      if(hidden[y][x] != 255){
        uint8_t count = 0;
        for(int i = 0; i<8;i++){
          int8_t x_mod = 0;
          int8_t y_mod = 0;
          switch(i){
          case 0:
          case 1:
          case 2:
            y_mod = -1;
            break;
          case 3:
          case 4:
            y_mod = 0;
            break;
          case 5:
          case 6:
          case 7:
            y_mod = 1;
            break;
          }
          switch(i){
          case 0:
          case 3:
          case 5:
            x_mod = -1;
            break;
          case 1:
          case 6:
            x_mod = 0;
            break;
          case 2:
          case 4:
          case 7:
            x_mod = 1;
            break;
          }
          if(x_mod + x >= 0 && x_mod + x < MAP_X && y_mod + y >= 0 && y_mod + y < MAP_Y
              && hidden[y+y_mod][x+x_mod] == 255
             ){
            count++;
          }
        }
        hidden[y][x] = count;
      }
    }
  }
}

void board::flag(){
  //TODO
  if(visable[cursor[dim_y]][cursor[dim_x]] == 254){
    visable[cursor[dim_y]][cursor[dim_x]] = 244;
  }else if(visable[cursor[dim_y]][cursor[dim_x]] == 244){
    visable[cursor[dim_y]][cursor[dim_x]] = 254;
  } 
}
int board::guess(){
  if(visable[cursor[dim_y]][cursor[dim_x]] == 244){
    return 0;
  }
  while(!started && hidden[cursor[dim_y]][cursor[dim_x]] != 0){
    reset();
    populate();
    set_proximity();
  }
  started = true;
  std::stack<pair_t*> stack;
  pair_t* cursor_cpy_ptr = (pair_t*) malloc(1 * sizeof(pair_t*));
  *cursor_cpy_ptr[dim_y] = cursor[dim_y];
  *cursor_cpy_ptr[dim_x] = cursor[dim_x];
  stack.push(cursor_cpy_ptr);
  while(!stack.empty()){
    pair_t* cur;
    cur = stack.top();
    stack.pop();

    if(visable[*cur[dim_y]][*cur[dim_x]] == 254 || visable[*cur[dim_y]][*cur[dim_x]] == 244){
      if(hidden[*cur[dim_y]][*cur[dim_x]] == 0){
        for(int i = 0; i<8;i++){
          int8_t x_mod = 0;
          int8_t y_mod = 0;
          switch(i){
          case 0:
          case 1:
          case 2:
            y_mod = -1;
            break;
          case 3:
          case 4:
            y_mod = 0;
            break;
          case 5:
          case 6:
          case 7:
            y_mod = 1;
            break;
          }
          switch(i){
          case 0:
          case 3:
          case 5:
            x_mod = -1;
            break;
          case 1:
          case 6:
            x_mod = 0;
            break;
          case 2:
          case 4:
          case 7:
            x_mod = 1;
            break;
          }
          if(*cur[dim_x]+x_mod >= 0 && *cur[dim_x]+x_mod < MAP_X
             && *cur[dim_y] + y_mod >=0 && *cur[dim_y] + y_mod < MAP_Y){
            pair_t* p = (pair_t*) malloc (1*sizeof(pair_t*));
            *p[dim_x] = (int16_t)((int)*cur[dim_x]+(int)x_mod);
            *p[dim_y] = (int16_t)((int)*cur[dim_y]+(int)y_mod);
            stack.push(p);
          }
        }
      }
      num_safe++;
      visable[*cur[dim_y]][*cur[dim_x]] = hidden[*cur[dim_y]][*cur[dim_x]];
    }
    free(cur);
  }
  // visable[cursor[dim_y]][cursor[dim_x]] = hidden[cursor[dim_y]][cursor[dim_x]];

  io_display_visable(this);
  if(visable[cursor[dim_y]][cursor[dim_x]] == 255){
    return -1;
  }else if(num_safe == MAP_Y * MAP_X - num_bombs){
    return 1;
  }else{
    return 0;
  }
}
// void board::print_hidden(){
//   clear();
//   for(int y = 0; y<MAP_Y; y++){
//     for(int x = 0; x < MAP_X; x++){
//       if(hidden[y][x]==255){
//         attron(COLOR_PAIR(COLOR_RED));
//         mvaddch(y+1,x, 'B');
//         attroff(COLOR_PAIR(COLOR_RED));
//       }else{
//         mvaddch(y+1, x, hidden[y][x]+48);
//       }
//     }
//   }
//   refresh();
// }
// void board::print_visable(){
//   clear();
//   for(int y = 0; y<MAP_Y; y++){
//     for(int x = 0; x < MAP_X; x++){
//       if(visable[y][x]==255){
//         attron(COLOR_PAIR(COLOR_RED));
//         mvaddch(y+1,x, 'B');
//         attroff(COLOR_PAIR(COLOR_RED));
//       }else{
//         mvaddch(y+1, x, visable[y][x]+48);
//       }
//     }
//   }
//   refresh();
// }
