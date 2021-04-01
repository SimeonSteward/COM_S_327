#include "mon_template.h"
#include "dice.h"
#include <string>
#include <sstream>
#include <iostream>


mon_template::mon_template(){
  name = "";
  description = "";
  color = static_cast<enum color>(8);
  speed = dice("0+0d0");
  abilities = 0;
  hp = dice("0+0d0");
  dam = dice("0+0d0");
  symb = '!';
  rrty = 0;
}
mon_template::mon_template(const mon_template &m){
  name = m.name;
  description = m.description;
  color = m.color;
  speed = m.speed;
  abilities = m.abilities;
  hp = m.hp;
  dam = m.dam;
  symb = m.symb;
  rrty = m.rrty;
}

std::string mon_template::to_string(){
  std::string ret_val;
  ret_val += name;
  ret_val += '\n';
  ret_val += description;
  ret_val += '\n';
  switch(color){
  case(BLACK):
    ret_val += "BLACK";
    break;
  case(RED):
    ret_val += "RED";
    break;
  case(GREEN):
    ret_val += "GREEN";
    break;
  case(YELLOW):
    ret_val += "YELLOW";
    break;
  case(BLUE):
    ret_val += "BLUE";
    break;
  case(MAGENTA):
    ret_val += "MAGENTA";
    break;
  case(CYAN):
    ret_val += "CYAN";
    break;
  case(WHITE):
    ret_val += "WHITE";
    break;
  default:
    std::cerr << "DOESNT HAVE VALID COLOR"<<std::endl;
    exit(-1);
  }
  ret_val += '\n';
  ret_val += speed.to_string();
  ret_val += '\n';
  ret_val += std::to_string(abilities);//TODO idk if this is exceptable format
  ret_val += '\n';
  ret_val += hp.to_string();
  ret_val += '\n';
  ret_val += dam.to_string();
  ret_val += '\n';
  ret_val += symb;
  ret_val += '\n';
  ret_val += std::to_string(rrty);
  ret_val += '\n';




  //ret_val << desc << '\n';
  //ret_val << 
  return ret_val;
}
