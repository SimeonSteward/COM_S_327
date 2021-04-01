#ifndef MON_TEMPLATE_H
# define MON_TEMPLATE_H

#include <string>
#include "dice.h"
enum ability{
             SMART,
             TELE,
             TUNNEL,
             ERRATIC,
             PASS,
             PICKUP,
             DESTROY,
             UNIQ,
             BOSS
};
enum color{
           BLACK = 0,
           RED,
           GREEN,
           YELLOW,
           BLUE,
           MAGENTA,
           CYAN,
           WHITE
};
class mon_template {
 public:
  std::string name;
  std::string description;
  enum color color;
  dice speed;
  int abilities;
  dice hp;
  dice dam;
  char symb;
  uint8_t rrty;
  mon_template();
  mon_template(const mon_template &m);
    
  std::string to_string();
    
};

#endif
  
