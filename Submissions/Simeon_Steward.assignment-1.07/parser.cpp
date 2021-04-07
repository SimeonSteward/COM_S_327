#include "parser.h"
#include "mon_template.h"
#include <string.h>


void parse_one_monster(dungeon &d, std::ifstream &i){
  char parse_string[256];
  i>>parse_string;
  mon_template new_monster = mon_template();
  while(strcmp(parse_string,"END")){
    //std::cout<<parse_string<<std::endl;
    if(!strcmp(parse_string,"BEGIN")){
      i.getline(parse_string,256);
    }else if(!strcmp(parse_string,"NAME")){
      i.getline(parse_string,256);
      new_monster.name = std::string(parse_string);
    }else if(!strcmp(parse_string,"DESC")){
      i.getline(parse_string,256);
      while(i.peek()!='.'){
        i.getline(parse_string,256);
        new_monster.description += std::string(parse_string);
        new_monster.description += '\n';
      }
      i.getline(parse_string,256);
    }else if(!strcmp(parse_string,"COLOR")){
      while(i.peek()!='\n'){
        i>>parse_string;
        if(!strcmp(parse_string,"BLACK")){
          new_monster.color.push_back(BLACK);
        }else if(!strcmp(parse_string,"RED")){
          new_monster.color.push_back(RED);
        }else if(!strcmp(parse_string,"GREEN")){
          new_monster.color.push_back(GREEN);
        }else if(!strcmp(parse_string,"YELLOW")){
          new_monster.color.push_back(YELLOW);
        }else if(!strcmp(parse_string,"BLUE")){
          new_monster.color.push_back(BLUE);
        }else if(!strcmp(parse_string,"MAGENTA")){
          new_monster.color.push_back(MAGENTA);
        }else if(!strcmp(parse_string,"CYAN")){
          new_monster.color.push_back(CYAN);
        }else if(!strcmp(parse_string,"WHITE")){
          new_monster.color.push_back(WHITE);
        }else{
          std::cerr<<"Not a valid color"<<std::endl;
          exit(-1);
        }
      }
      i.getline(parse_string,256);
    }else if(!strcmp(parse_string,"SPEED")){
      i.getline(parse_string,256);
      new_monster.speed = dice(std::string(parse_string));
    }else if(!strcmp(parse_string,"ABIL")){
      while(i.peek()!='\n'){
        i>>parse_string;
        if(!strcmp(parse_string,"SMART")){
          new_monster.abilities |= (1<<0);
        }else if(!strcmp(parse_string,"TELE")){
          new_monster.abilities |= (1<<1);
        }else if(!strcmp(parse_string,"TUNNEL")){
          new_monster.abilities |= (1<<2);
        }else if(!strcmp(parse_string,"ERRATIC")){
          new_monster.abilities |= (1<<3);
        }else if(!strcmp(parse_string,"PASS")){
          new_monster.abilities |= (1<<4);
        }else if(!strcmp(parse_string,"PICKUP")){
          new_monster.abilities |= (1<<5);
        }else if(!strcmp(parse_string,"DESTROY")){
          new_monster.abilities |= (1<<6);
        }else if(!strcmp(parse_string,"UNIQ")){
          new_monster.abilities |= (1<<7);
        }else if(!strcmp(parse_string,"BOSS")){
          new_monster.abilities |= (1<<8);
        }
      }
    }else if(!strcmp(parse_string,"HP")){
      i.getline(parse_string,256);
      new_monster.hp = dice(std::string(parse_string));
    }else if(!strcmp(parse_string,"DAM")){
      i.getline(parse_string,256);
      new_monster.dam = dice(std::string(parse_string));
    }else if(!strcmp(parse_string,"SYMB")){
      i.get();
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
