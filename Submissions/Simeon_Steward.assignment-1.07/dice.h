#ifndef DICE_H
# define DICE_H
#include <stdlib.h>
#include <string>
class dice {
 private:
  //b+adx example: 12+1d10
  int a,x,b;
 public:
  dice();
  dice(std::string unparsed);
  dice(int a_in, int x_in, int b_in);
  ~dice();
  std::string to_string()const;
  int roll()const;

};

#endif
