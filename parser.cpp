#include "parser.h"
#include "mon_template.h"
#include <string.h>


void parse_one_monster(dungeon &d, std::ifstream &i){
  char parse_string[256];
  i>>parse_string;
  mon_template new_monster = mon_template();
  while(strcmp(parse_string,"END")){
    std::cout<<parse_string<<std::endl;
    if(!strcmp(parse_string,"BEGIN")){
      i.getline(parse_string,256);
    }else if(!strcmp(parse_string,"NAME")){
      i.getline(parse_string,256);
      new_monster.name = std::string(parse_string);
    }else if(!strcmp(parse_string,"DESC")){
      while(i.peek()!='.'){
        i.getline(parse_string,256);
        new_monster.description += std::string(parse_string);
      }
      i.getline(parse_string,256);
    }else if(!strcmp(parse_string,"COLOR")){
      i>>parse_string;
      if(!strcmp(parse_string,"BLACK")){
        new_monster.color = BLACK;
      }else if(!strcmp(parse_string,"RED")){
        new_monster.color = RED;
      }else if(!strcmp(parse_string,"GREEN")){
        new_monster.color = GREEN;
      }else if(!strcmp(parse_string,"YELLOW")){
        new_monster.color = YELLOW;
      }else if(!strcmp(parse_string,"BLUE")){
        new_monster.color = BLUE;
      }else if(!strcmp(parse_string,"MAGENTA")){
        new_monster.color = MAGENTA;
      }else if(!strcmp(parse_string,"CYAN")){
        new_monster.color = CYAN;
      }else if(!strcmp(parse_string,"WHITE")){
        new_monster.color = WHITE;
      }else{
        std::cerr<<"Not a valid color"<<std::endl;
        exit(-1);
      }
      i.getline(parse_string,256);
    }else if(!strcmp(parse_string,"SPEED")){
      i.getline(parse_string,256);
      new_monster.speed = dice(std::string(parse_string));
    }else if(!strcmp(parse_string,"ABIL")){
      while(i.peek()!='\n'){
        i>>parse_string;
        if(strcmp(parse_string,"SMART")){
          new_monster.abilities &= 0x00000001;
        }else if(strcmp(parse_string,"TELE")){
          new_monster.abilities &= 0x00000002;
        }else if(strcmp(parse_string,"TUNNEL")){
          new_monster.abilities &= 0x00000004;
        }else if(strcmp(parse_string,"ERRATIC")){
          new_monster.abilities &= 0x00000008;
        }else if(strcmp(parse_string,"PASS")){
          new_monster.abilities &= 0x00000016;
        }else if(strcmp(parse_string,"PICKUP")){
          new_monster.abilities &= 0x00000032;
        }else if(strcmp(parse_string,"DESTROY")){
          new_monster.abilities &= 0x00000064;
        }else if(strcmp(parse_string,"UNIQ")){
          new_monster.abilities &= 0x00000128;
        }else if(strcmp(parse_string,"BOSS")){
          new_monster.abilities &= 0x00000256;
        }
      }
    }else if(!strcmp(parse_string,"HP")){
      i.getline(parse_string,256);
      new_monster.hp = dice(std::string(parse_string));
    }else if(!strcmp(parse_string,"DAM")){
      i.getline(parse_string,256);
      new_monster.dam = dice(std::string(parse_string));
    }else if(!strcmp(parse_string,"SYMB")){
      new_monster.symb = i.get();
      i.getline(parse_string,256);
    }else if(!strcmp(parse_string,"RRTY")){
      i>>parse_string;
      new_monster.rrty = std::stoi(parse_string);
      i.getline(parse_string,256);
    }
    i>>parse_string;
  }
  d.monster_templates.push_back(new_monster);
}
void parse_file(dungeon &d,std::ifstream &i){
  char parse_string[256];
  i.getline(parse_string,256);
  std::cout << parse_string<<std::endl;
  i.getline(parse_string,256);//gets rid of whitespace
  while(i.good()){
    parse_one_monster(d,i);
  }
}
