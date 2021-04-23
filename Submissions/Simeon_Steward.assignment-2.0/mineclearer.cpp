#include <time.h>
#include <cstring>
#include "board.h"
#include "io.h"
void initialize_terminal(){
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}
void reset_terminal(void)
{
  endwin();
}

int main(int argc, char *argv[]){
  int16_t num_bombs = 200;
  if(argc >= 3 && !std::strcmp("--numbomb",argv[1])){
    num_bombs = atoi(argv[2]);
  }
  io_init_terminal();
  srand(time(NULL));
  board b = board(num_bombs);
  b.populate();
  b.set_proximity();
  initialize_terminal();
  //b.print_hidden();
  //getch();
  io_display_visable(&b);
  refresh();

  //io_handle_input(&b);
  
  switch (io_handle_input(&b)){
  case 1:
    io_queue_message("Congrats you won!");
    break;
  case -1:
    io_queue_message("You exploded and lost!");
    break;
  case -2:
    io_queue_message("You gave up.");
    break;
  };
  io_display_hidden(&b);
  getch();
  reset_terminal();
}
