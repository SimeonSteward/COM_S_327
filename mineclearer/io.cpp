#include <stdlib.h>
#include <ncurses.h>

#include "board.h"
#include "io.h"
#define CURSOR_PAIR 8

typedef struct io_message {
  /* Will print " --more-- " at end of line when another message follows. *
   * Leave 10 extra spaces for that.                                      */
  char msg[71];
  struct io_message *next;
} io_message_t;

static io_message_t *io_head, *io_tail;

void io_init_terminal(void)
{
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
  init_pair(CURSOR_PAIR, COLOR_YELLOW, COLOR_RED);
}

void io_reset_terminal(void)
{
  endwin();

  while (io_head) {
    io_tail = io_head;
    io_head = io_head->next;
    free(io_tail);
  }
  io_tail = NULL;
}

void io_queue_message(const char *format, ...)
{
  io_message_t *tmp;
  va_list ap;

  if (!(tmp = (io_message_t *) malloc(sizeof (*tmp)))) {
    perror("malloc");
    exit(1);
  }

  tmp->next = NULL;

  va_start(ap, format);

  vsnprintf(tmp->msg, sizeof (tmp->msg), format, ap);

  va_end(ap);

  if (!io_head) {
    io_head = io_tail = tmp;
  } else {
    io_tail->next = tmp;
    io_tail = tmp;
  }
}

static void io_print_message_queue(uint32_t y, uint32_t x)
{
  while (io_head) {
    io_tail = io_head;
    attron(COLOR_PAIR(COLOR_CYAN));
    mvprintw(y, x, "%-80s", io_head->msg);
    attroff(COLOR_PAIR(COLOR_CYAN));
    io_head = io_head->next;
    if (io_head) {
      attron(COLOR_PAIR(COLOR_CYAN));
      mvprintw(y, x + 70, "%10s", " --more-- ");
      attroff(COLOR_PAIR(COLOR_CYAN));
      refresh();
      getch();
    }
    free(io_tail);
  }
  io_tail = NULL;
}

void io_display_hidden(board *b){
  clear();
  for(int y = 0; y<MAP_Y; y++){
    for(int x = 0; x < MAP_X; x++){
      switch(b->hidden[y][x]){
      case 255:
        attron(COLOR_PAIR(COLOR_RED));
        mvaddch(y+1,x, 'B');
        attroff(COLOR_PAIR(COLOR_RED));
        break;
      case 254:
        mvaddch(y+1,x,'*');
        break;
      case 244:
        attron(COLOR_PAIR(COLOR_RED));
        mvaddch(y+1,x, 'F');
        attroff(COLOR_PAIR(COLOR_RED));
        break;
      case 1:
        attron(COLOR_PAIR(COLOR_YELLOW));
        mvaddch(y+1,x, b->hidden[y][x]+48);
        attroff(COLOR_PAIR(COLOR_YELLOW));
        break;
      case 2:
        attron(COLOR_PAIR(COLOR_GREEN));
        mvaddch(y+1,x, b->hidden[y][x]+48);
        attroff(COLOR_PAIR(COLOR_GREEN));
        break;
      case 3:
        attron(COLOR_PAIR(COLOR_BLUE));
        mvaddch(y+1,x, b->hidden[y][x]+48);
        attroff(COLOR_PAIR(COLOR_BLUE));
        break;
      case 4:
        attron(COLOR_PAIR(COLOR_MAGENTA));
        mvaddch(y+1,x, b->hidden[y][x]+48);
        attroff(COLOR_PAIR(COLOR_MAGENTA));
        break;
      case 5:
        attron(COLOR_PAIR(COLOR_CYAN));
        mvaddch(y+1,x, b->hidden[y][x]+48);
        attroff(COLOR_PAIR(COLOR_CYAN));
        break;
      case 6:
        attron(COLOR_PAIR(COLOR_CYAN));
        mvaddch(y+1,x, b->hidden[y][x]+48);
        attroff(COLOR_PAIR(COLOR_CYAN));
        break;
      case 7:
        attron(COLOR_PAIR(COLOR_CYAN));
        mvaddch(y+1,x, b->hidden[y][x]+48);
        attroff(COLOR_PAIR(COLOR_CYAN));
        break;
      case 8:
        attron(COLOR_PAIR(COLOR_CYAN));
        mvaddch(y+1,x, b->hidden[y][x]+48);
        attroff(COLOR_PAIR(COLOR_CYAN));
        break;
      default:
        mvaddch(y+1, x, b->hidden[y][x]+48);
        break;
      }

    }
  }
  attron(COLOR_PAIR(COLOR_CYAN));
  mvprintw(MAP_Y+1,0,"Safe: %d Target: %d", b->num_safe, MAP_Y * MAP_X - b->num_bombs);
  attroff(COLOR_PAIR(COLOR_CYAN));
  io_print_message_queue(0,0);
  refresh();
}

void io_display_visable(board *b){
  //attron(A_NORMAL);
  attroff(COLOR_PAIR(COLOR_RED));
  clear();
  for(int y = 0; y<MAP_Y; y++){
    for(int x = 0; x < MAP_X; x++){
      switch(b->visable[y][x]){
      case 255:
        attron(COLOR_PAIR(COLOR_RED));
        mvaddch(y+1,x, 'B');
        attroff(COLOR_PAIR(COLOR_RED));
        break;
      case 254:
        mvaddch(y+1,x,'*');
        break;
      case 244:
        attron(COLOR_PAIR(COLOR_RED));
        mvaddch(y+1,x, 'F');
        attroff(COLOR_PAIR(COLOR_RED));
        break;
      case 1:
        attron(COLOR_PAIR(COLOR_YELLOW));
        mvaddch(y+1,x, b->visable[y][x]+48);
        attroff(COLOR_PAIR(COLOR_YELLOW));
        break;
      case 2:
        attron(COLOR_PAIR(COLOR_GREEN));
        mvaddch(y+1,x, b->visable[y][x]+48);
        attroff(COLOR_PAIR(COLOR_GREEN));
        break;
      case 3:
        attron(COLOR_PAIR(COLOR_BLUE));
        mvaddch(y+1,x, b->visable[y][x]+48);
        attroff(COLOR_PAIR(COLOR_BLUE));
        break;
      case 4:
        attron(COLOR_PAIR(COLOR_MAGENTA));
        mvaddch(y+1,x, b->visable[y][x]+48);
        attroff(COLOR_PAIR(COLOR_MAGENTA));
        break;
      case 5:
        attron(COLOR_PAIR(COLOR_CYAN));
        mvaddch(y+1,x, b->visable[y][x]+48);
        attroff(COLOR_PAIR(COLOR_CYAN));
        break;
      case 6:
        attron(COLOR_PAIR(COLOR_CYAN));
        mvaddch(y+1,x, b->visable[y][x]+48);
        attroff(COLOR_PAIR(COLOR_CYAN));
        break;
      case 7:
        attron(COLOR_PAIR(COLOR_CYAN));
        mvaddch(y+1,x, b->visable[y][x]+48);
        attroff(COLOR_PAIR(COLOR_CYAN));
        break;
      case 8:
        attron(COLOR_PAIR(COLOR_CYAN));
        mvaddch(y+1,x, b->visable[y][x]+48);
        attroff(COLOR_PAIR(COLOR_CYAN));
        break;
      default:
        mvaddch(y+1, x, b->visable[y][x]+48);
        break;
      }
    }
  }
  attron(COLOR_PAIR(COLOR_CYAN));
  mvprintw(MAP_Y+1,0,"Safe: %d Target: %d", b->num_safe, MAP_Y * MAP_X - b->num_bombs);
  attroff(COLOR_PAIR(COLOR_CYAN));
  io_print_message_queue(0,0);
  refresh();
}
void hide_cursor(board *b){
  //TODO
  //really inefficient way to do this, maybe optimize later
  io_display_visable(b);
}
void print_cursor(board *b){
  attron(COLOR_PAIR(CURSOR_PAIR));
  switch(b->visable[b->cursor[dim_y]][b->cursor[dim_x]]){
  case 255:
    mvaddch(b->cursor[dim_y]+1,b->cursor[dim_x], 'B');
    break;
  case 254:
    mvaddch(b->cursor[dim_y]+1,b->cursor[dim_x],'*');
    break;
  case 244:
    mvaddch(b->cursor[dim_y]+1,b->cursor[dim_x],'F');
    break;
  default:
    mvaddch(b->cursor[dim_y]+1, b->cursor[dim_x], b->visable[b->cursor[dim_y]][b->cursor[dim_x]]+48);
    break;
  }
  //io_queue_message("Y: %d, X: %d", b->cursor[dim_y],b->cursor[dim_x]);
  //io_print_message_queue(0,0);
  attroff(COLOR_PAIR(CURSOR_PAIR));
  refresh();
}
int io_handle_input(board *b){
  //assume visable is already printed
  int r = 0;
  //loop until location is choosen
  print_cursor(b);
  while(!r){
    //hide cursor
    int s = getch();
    hide_cursor(b);

    switch(s){
    case '7':
    case 'y':
    case KEY_HOME:
      if (b->cursor[dim_y] != 0) {
        b->cursor[dim_y]--;
      }
      if (b->cursor[dim_x] != 0) {
        b->cursor[dim_x]--;
      }
      break;
    case '8':
    case 'k':
    case KEY_UP:
      if (b->cursor[dim_y] != 0) {
        b->cursor[dim_y]--;
      }
      break;
    case '9':
    case 'u':
    case KEY_PPAGE:
      if (b->cursor[dim_y] != 0) {
        b->cursor[dim_y]--;
      }
      if (b->cursor[dim_x] != MAP_X - 1) {
        b->cursor[dim_x]++;
      }
      break;
    case '6':
    case 'l':
    case KEY_RIGHT:
      if (b->cursor[dim_x] != MAP_X - 1) {
        b->cursor[dim_x]++;
      }
      break;
    case '3':
    case 'n':
    case KEY_NPAGE:
      if (b->cursor[dim_y] != MAP_Y - 1) {
        b->cursor[dim_y]++;
      }
      if (b->cursor[dim_x] != MAP_X - 1) {
        b->cursor[dim_x]++;
      }
      break;
    case '2':
    case 'j':
    case KEY_DOWN:
      if (b->cursor[dim_y] != MAP_Y - 1) {
        b->cursor[dim_y]++;
      }
      break;
    case '1':
    case 'b':
    case KEY_END:
      if (b->cursor[dim_y] != MAP_Y - 1) {
        b->cursor[dim_y]++;
      }
      if (b->cursor[dim_x] != 0) {
        b->cursor[dim_x]--;
      }
      break;
    case '4':
    case 'h':
    case KEY_LEFT:
      if (b->cursor[dim_x] != 0) {
        b->cursor[dim_x]--;
      }
      break;
    case 'f':
      b->flag();
      break;
    case 'g':
      r = b->guess();
      break;
    case 'q':
    case 'Q':
      r = -2;
      break;
      
    }

    print_cursor(b);
  }
  return r;
}
