#ifndef IO_H
# define IO_H
#include <string>

class dungeon;

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(dungeon *d);
void io_handle_input(dungeon *d);
void io_queue_message(const char *format, ...);
void io_queue_message(std::string s);

#endif
