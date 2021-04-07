#include "dice.h"
#include <string>
#include <sstream>
#include <iostream>


dice::dice(){
  a = 0;
  x = 0;
  b = 0;
 }
dice::dice(int a_in, int x_in, int b_in){
  a = a_in;
  x = x_in;
  b = b_in;
}
/*
 *Takes a dice in string form and parses it b+adx example: 12+2d10
 */
dice::dice(std::string unparsed){
  std::stringstream ss;
  char parse[256];
  ss << unparsed;
  ss.getline(parse,256,'+');
  b = std::stoi(parse);
  ss.getline(parse,256,'d');
  a = std::stoi(parse);
  ss.getline(parse,256);
  x = std::stoi(parse);

}
dice::~dice(){
}
std::string dice::to_string()const{
  std::string ret_val;
  ret_val = std::to_string(b)+'+'+std::to_string(a)+'d'+std::to_string(x);
  return ret_val;
}
int dice::roll()const{
  int sum = b;
  for(int i = 0;i<a;i++){
    sum += rand()%10;
  }
  return sum;
}
