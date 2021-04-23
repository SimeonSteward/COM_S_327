#ifndef IO_H
# define IO_H

class board;

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display_hidden(board *b);
void io_display_visable(board *b);
int io_handle_input(board *b);
void io_queue_message(const char *format, ...);
#endif
