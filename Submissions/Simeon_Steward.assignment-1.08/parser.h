#ifndef PARSER_H
# define PARSER_H
#include <iostream>
#include <fstream>
#include "dungeon.h"
void parse_one_monster(dungeon &d, std::ifstream &i);
void parse_file(dungeon &d,std::ifstream &i);

#endif
