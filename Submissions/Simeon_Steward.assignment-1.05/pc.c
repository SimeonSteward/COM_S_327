#include <stdlib.h>
#include <ncurses.h>


#include "string.h"
#include "npc.h"
#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"
#include "character.h"

void pc_delete(pc_t *pc)
{
  if (pc) {
    free(pc);
  }
}

uint32_t pc_is_alive(dungeon_t *d)
{
  return d->pc.alive;
}

void place_pc(dungeon_t *d)
{
  d->pc.position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->pc.position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));
  d->character[dim_y][dim_x] = &(d->pc);
}

void config_pc(dungeon_t *d)
{
  memset(&d->pc, 0, sizeof (d->pc));
  d->pc.symbol = '@';

  place_pc(d);

  d->pc.speed = PC_SPEED;
  d->pc.alive = 1;
  d->pc.sequence_number = 0;
  d->pc.pc = calloc(1, sizeof (*d->pc.pc));
  d->pc.npc = NULL;
  d->pc.kills[kill_direct] = d->pc.kills[kill_avenged] = 0;

  d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = &d->pc;

  dijkstra(d);
  dijkstra_tunnel(d);
}
/*
uint32_t pc_next_pos(dungeon_t *d, pair_t dir)
{
  static uint32_t have_seen_corner = 0;
  static uint32_t count = 0;
  static int target_room = -1;
  static int target_is_valid = 0;

  if (target_is_valid &&
      (d->pc.position[dim_x] == d->rooms[target_room].position[dim_x]) &&
      (d->pc.position[dim_y] == d->rooms[target_room].position[dim_y])) {
    target_is_valid = 0;
  }
  
  dir[dim_y] = dir[dim_x] = 0;

  if (in_corner(d, &d->pc)) {
    if (!count) {
      count = 1;
    }
    have_seen_corner = 1;
  }

  //First, eat anybody standing next to us. 
  if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = -1;
  } else if (charxy(d->pc.position[dim_x], d->pc.position[dim_y] - 1)) {
    dir[dim_y] = -1;
  } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = 1;
  } else if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y])) {
    dir[dim_x] = -1;
  } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y])) {
    dir[dim_x] = 1;
  } else if (charxy(d->pc.position[dim_x] - 1, d->pc.position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = -1;
  } else if (charxy(d->pc.position[dim_x], d->pc.position[dim_y] + 1)) {
    dir[dim_y] = 1;
  } else if (charxy(d->pc.position[dim_x] + 1, d->pc.position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = 1;
  } else if (!have_seen_corner || count < 250) {
    // Head to a corner and let most of the NPCs kill each other off
    if (count) {
      count++;
    }
    if (!against_wall(d, &d->pc) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir_nearest_wall(d, &d->pc, dir);
    }
  } else {
    // And after we've been there, let's cycle through the rooms, 
    // one-by-one, until the game ends                           
    if (target_room == -1) {
      target_room = 0;
      target_is_valid = 1;
    }
    if (!target_is_valid) {
      target_is_valid = 1;
      target_room = (target_room + 1) % d->num_rooms;
    }
// When against the dungeon border, always head toward the target;
// otherwise, head toward the target with 1/3 probability.        
    if (against_wall(d, &d->pc) || rand_under(1, 3)) {
      dir[dim_x] = ((d->pc.position[dim_x] >
		     d->rooms[target_room].position[dim_x]) ? -1 : 1);
      dir[dim_y] = ((d->pc.position[dim_y] >
		     d->rooms[target_room].position[dim_y]) ? -1 : 1);
    } else {
      // Else we'll choose a random direction 
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    }
  }

  // Don't move to an unoccupied location if that places us next to a monster
  if (!charxy(d->pc.position[dim_x] + dir[dim_x],
              d->pc.position[dim_y] + dir[dim_y]) &&
      ((charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
               d->pc.position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
                d->pc.position[dim_y] + dir[dim_y] - 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
               d->pc.position[dim_y] + dir[dim_y]) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
                d->pc.position[dim_y] + dir[dim_y]) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
               d->pc.position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] - 1,
                d->pc.position[dim_y] + dir[dim_y] + 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x],
               d->pc.position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x],
                d->pc.position[dim_y] + dir[dim_y] - 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x],
               d->pc.position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x],
                d->pc.position[dim_y] + dir[dim_y] + 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
               d->pc.position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
                d->pc.position[dim_y] + dir[dim_y] - 1) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
               d->pc.position[dim_y] + dir[dim_y]) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
                d->pc.position[dim_y] + dir[dim_y]) != &d->pc)) ||
       (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
               d->pc.position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->pc.position[dim_x] + dir[dim_x] + 1,
                d->pc.position[dim_y] + dir[dim_y] + 1) != &d->pc)))) {
    dir[dim_x] = dir[dim_y] = 0;
  }

  return 0;
}
*/
uint32_t pc_next_pos(dungeon_t *d, pair_t dir){
  int success = 0;
  d->virgin = 0;
  
  while(!success){
    success = 1;
    int charIn = getch();
    switch(charIn){
    case(0171)://y
    case(067)://7
      dir[dim_x] = -1;
      dir[dim_y] = -1;
      break;
    case(0153)://k
    case(070)://8
      dir[dim_x] = 0;
      dir[dim_y] = -1;
      break;
    case(0165)://u
    case(071)://9
      dir[dim_x] = 1;
      dir[dim_y] = -1;
      break;
    case(0154)://l
    case(066)://6
      dir[dim_x] = 1;
      dir[dim_y] = 0;
      break;
    case(0156)://n
    case(063)://3
      dir[dim_x] = 1;
      dir[dim_y] = 1;
      break;
    case(0152)://j
    case(062)://2
      dir[dim_x] = 0;
      dir[dim_y] = 1;
      break;
    case(0142)://b
    case(061)://1
      dir[dim_x] = -1;
      dir[dim_y] = 1;
      break;
    case(0150)://h
    case(064)://4
      dir[dim_x] = -1;
      dir[dim_y] = 0;
      break;
    case(065)://5
    case(040)://space
    case(056)://.
      dir[dim_x] = 0;
      dir[dim_y] = 0;
      break;
    case(076)://>
      if(d->map[d->pc.position[dim_y]][d->pc.position[dim_x]]!=ter_stairs_down){
        success = 0;
        mvprintw(DUNGEON_Y+1,40,"Error:No up stairs here                 ");
        break;
      }else{
        delete_dungeon(d);
        init_dungeon(d);
        gen_dungeon(d);
        place_pc(d);
        gen_monsters(d);
        d->virgin = 1;
        render_dungeon(d);
        break;
      }
    case(074)://<
      if(d->map[d->pc.position[dim_y]][d->pc.position[dim_x]]!=ter_stairs_up){
      success = 0;
      mvprintw(DUNGEON_Y+1,40,"Error:No down stairs here                 ");
      break;
      }else{
      delete_dungeon(d);
      init_dungeon(d);
      gen_dungeon(d);
      place_pc(d);
      gen_monsters(d);
      d->virgin = 1;
      render_dungeon(d);
      break;
      }
    case(0121)://Q
    case(0161)://q
      clear();
      endwin();
      delete_dungeon(d);
      exit(0);
      break;
    case(0155):;//m
      success = 0;
      character_t *monsterList;
      monsterList= malloc(sizeof(character_t)*d->num_monsters);
      int i = 0;
      int menu = 1;
      pair_t pos;
      for(pos[dim_y] = 0;pos[dim_y]<DUNGEON_Y;pos[dim_y]++){
        for(pos[dim_x] = 0;pos[dim_x]<DUNGEON_X;pos[dim_x]++){
          if(d->character[pos[dim_y]][pos[dim_x]]&&d->character[pos[dim_y]][pos[dim_x]]->symbol!='@'){
            monsterList[i++] = *(d->character[pos[dim_y]][pos[dim_x]]);//as character is an array of character pointers, and monsterList is an array of characters, need to dereference it
          }
        }
      }
      int offset = 0;
        while(menu){
          for(int i = 0;i<(d->num_monsters<MENU_SIZE?d->num_monsters:MENU_SIZE);i++){
            pair_t p;
            relative_pos_to_pc(d,&monsterList[i+offset],&p);
            mvprintw(i+5,20,"     Monster %c at %d %c %d %c    ",
                     monsterList[i+offset].symbol,p[dim_y]>0?p[dim_y]:-1*p[dim_y],p[dim_y]>0?'N':'S',p[dim_x]>0?p[dim_x]:-1*p[dim_x],p[dim_x]>0?'E':'W');
          } 
          refresh();

          charIn = getch();
          switch(charIn){
          case(033)://esc
            menu = 0;
            render_dungeon(d);
            break;
          case(0403)://up
            //offset --;
            offset = (offset-1>0?offset-1:0);
            //mvprintw(DUNGEON_Y+1,40,"Offset: %d",offset);
;
            break;
          case(0402)://down
            offset = (offset+1+MENU_SIZE>d->num_monsters?offset:offset+1);
            //offset++;
            //mvprintw(DUNGEON_Y+1,40,"Offset: %d",offset);
            break;
          default:
            mvprintw(DUNGEON_Y+1,40,"Error: Invalid character 0%o pressed",charIn);
          }
        }
      break;
    default:
      success = 0;
      mvprintw(DUNGEON_Y+1,40,"Error: Invalid character 0%o pressed",charIn);
    }
    if(d->map[d->pc.position[dim_y]+dir[dim_y]][d->pc.position[dim_x]+dir[dim_x]]==ter_wall||
       d->map[d->pc.position[dim_y]+dir[dim_y]][d->pc.position[dim_x]+dir[dim_x]]==ter_wall_immutable
       ){
      success = 0;
      mvprintw(DUNGEON_Y+1,40,"Error: Can't travel into wall");
    }
  }
  mvprintw(DUNGEON_Y+1,40,"                                         ");
  return 0;
}
  uint32_t pc_in_room(dungeon_t *d, uint32_t room)
  {
    if ((room < d->num_rooms)                                     &&
        (d->pc.position[dim_x] >= d->rooms[room].position[dim_x]) &&
        (d->pc.position[dim_x] < (d->rooms[room].position[dim_x] +
                                  d->rooms[room].size[dim_x]))    &&
        (d->pc.position[dim_y] >= d->rooms[room].position[dim_y]) &&
        (d->pc.position[dim_y] < (d->rooms[room].position[dim_y] +
                                  d->rooms[room].size[dim_y]))) {
      return 1;
    }

    return 0;
  }
