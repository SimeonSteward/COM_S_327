#include <cstdlib>
#include <cstring>
#include <ncurses.h>

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"
#include "io.h"
#include "object.h"

pc::pc(){
  weapon_slot = 0;
  offhand_slot = 0;
  ranged_slot = 0;
  armor_slot = 0;
  helmet_slot = 0;
  cloak_slot = 0;
  gloves_slot = 0;
  boots_slot = 0;
  amulet_slot = 0;
  light_slot = 0;
  ring_one_slot = 0;
  ring_two_slot = 0;
  }
uint32_t pc::get_damage(){
  
  uint32_t damage_dealt = this->damage->roll();
  if(weapon_slot){
    damage_dealt+=weapon_slot->roll_dice();
  }
  if(offhand_slot){
    damage_dealt+=offhand_slot->roll_dice();
  }
  if(ranged_slot){
    damage_dealt+=ranged_slot->roll_dice();
  }
  if(armor_slot){
    damage_dealt+=armor_slot->roll_dice();
  }
  if(helmet_slot){
    damage_dealt+=helmet_slot->roll_dice();
  }
  if(cloak_slot){
    damage_dealt+=cloak_slot->roll_dice();
  }
  if(gloves_slot){
    damage_dealt+=gloves_slot->roll_dice();
  }
  if(boots_slot){
    damage_dealt+=boots_slot->roll_dice();
  }
  if(amulet_slot){
    damage_dealt+=amulet_slot->roll_dice();
  }
  if(light_slot){
    damage_dealt+=light_slot->roll_dice();
  }
  if(ring_one_slot){
    damage_dealt+=ring_one_slot->roll_dice();
  }
  if(ring_two_slot){
    damage_dealt+=ring_two_slot->roll_dice();
  }
  return damage_dealt;
}
uint32_t pc_is_alive(dungeon *d)
{
  return d->PC->alive;
}

void place_pc(dungeon *d)
{
  d->PC->position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->PC->position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));

  pc_init_known_terrain(d->PC);
  pc_observe_terrain(d->PC, d);
}

void config_pc(dungeon *d)
{
  static dice pc_dice(0, 1, 4);
  
  d->PC = new pc;

  d->PC->symbol = '@';

  place_pc(d);

  d->PC->speed = PC_SPEED;
  d->PC->alive = 1;
  d->PC->sequence_number = 0;
  d->PC->kills[kill_direct] = d->PC->kills[kill_avenged] = 0;
  d->PC->color.push_back(COLOR_WHITE);
  d->PC->damage = &pc_dice;
  d->PC->name = "Isabella Garcia-Shapiro";
  d->PC->weapon_slot = NULL;
  d->PC->offhand_slot = NULL;
  d->PC->hp = 100;

  d->character_map[d->PC->position[dim_y]][d->PC->position[dim_x]] = d->PC;

  dijkstra(d);
  dijkstra_tunnel(d);
}

uint32_t pc_next_pos(dungeon *d, pair_t dir)
{
  static uint32_t have_seen_corner = 0;
  static uint32_t count = 0;

  dir[dim_y] = dir[dim_x] = 0;

  if (in_corner(d, d->PC)) {
    if (!count) {
      count = 1;
    }
    have_seen_corner = 1;
  }

  /* First, eat anybody standing next to us. */
  if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y])) {
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y])) {
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = 1;
  } else if (!have_seen_corner || count < 250) {
    /* Head to a corner and let most of the NPCs kill each other off */
    if (count) {
      count++;
    }
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir_nearest_wall(d, d->PC, dir);
    }
  }else {
    /* And after we've been there, let's head toward the center of the map. */
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir[dim_x] = ((d->PC->position[dim_x] > DUNGEON_X / 2) ? -1 : 1);
      dir[dim_y] = ((d->PC->position[dim_y] > DUNGEON_Y / 2) ? -1 : 1);
    }
  }

  /* Don't move to an unoccupied location if that places us next to a monster */
  if (!charxy(d->PC->position[dim_x] + dir[dim_x],
              d->PC->position[dim_y] + dir[dim_y]) &&
      ((charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)))) {
    dir[dim_x] = dir[dim_y] = 0;
  }

  return 0;
}

uint32_t pc_in_room(dungeon *d, uint32_t room)
{
  if ((room < d->num_rooms)                                     &&
      (d->PC->position[dim_x] >= d->rooms[room].position[dim_x]) &&
      (d->PC->position[dim_x] < (d->rooms[room].position[dim_x] +
                                d->rooms[room].size[dim_x]))    &&
      (d->PC->position[dim_y] >= d->rooms[room].position[dim_y]) &&
      (d->PC->position[dim_y] < (d->rooms[room].position[dim_y] +
                                d->rooms[room].size[dim_y]))) {
    return 1;
  }

  return 0;
}

void pc_learn_terrain(pc *p, pair_t pos, terrain_type ter)
{
  p->known_terrain[pos[dim_y]][pos[dim_x]] = ter;
  p->visible[pos[dim_y]][pos[dim_x]] = 1;
}

void pc_reset_visibility(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->visible[y][x] = 0;
    }
  }
}

terrain_type pc_learned_terrain(pc *p, int16_t y, int16_t x)
{
  if (y < 0 || y >= DUNGEON_Y || x < 0 || x >= DUNGEON_X) {
    io_queue_message("Invalid value to %s: %d, %d", __FUNCTION__, y, x);
  }

  return p->known_terrain[y][x];
}

void pc_init_known_terrain(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->known_terrain[y][x] = ter_unknown;
      p->visible[y][x] = 0;
    }
  }
}

void pc_observe_terrain(pc *p, dungeon *d)
{
  pair_t where;
  int16_t y_min, y_max, x_min, x_max;

  y_min = p->position[dim_y] - PC_VISUAL_RANGE;
  if (y_min < 0) {
    y_min = 0;
  }
  y_max = p->position[dim_y] + PC_VISUAL_RANGE;
  if (y_max > DUNGEON_Y - 1) {
    y_max = DUNGEON_Y - 1;
  }
  x_min = p->position[dim_x] - PC_VISUAL_RANGE;
  if (x_min < 0) {
    x_min = 0;
  }
  x_max = p->position[dim_x] + PC_VISUAL_RANGE;
  if (x_max > DUNGEON_X - 1) {
    x_max = DUNGEON_X - 1;
  }

  for (where[dim_y] = y_min; where[dim_y] <= y_max; where[dim_y]++) {
    where[dim_x] = x_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_x] = x_max;
    can_see(d, p->position, where, 1, 1);
  }
  /* Take one off the x range because we alreay hit the corners above. */
  for (where[dim_x] = x_min - 1; where[dim_x] <= x_max - 1; where[dim_x]++) {
    where[dim_y] = y_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_y] = y_max;
    can_see(d, p->position, where, 1, 1);
  }       
}

int32_t is_illuminated(pc *p, int16_t y, int16_t x)
{
  return p->visible[y][x];
}

void pc_see_object(character *the_pc, object *o)
{
  if (o) {
    o->has_been_seen();
  }
}

void pc_drop(dungeon *d, int slot){
  if(slot>=0&&slot<(int)d->PC->carry.size()){
    object* obj = d->PC->carry[slot];
    std::vector<object*> &carry = d->PC->carry;
    d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]]=obj;
    carry.erase(carry.begin()+slot);
  }
}
void pc_expunge(dungeon *d, int slot){
  if(slot>=0&&slot<(int)d->PC->carry.size()){
    std::vector<object*> &carry = d->PC->carry;
    carry.erase(carry.begin()+slot);
  }
}
void pc_wear(dungeon *d, int slot){
  if(slot>=0&&slot<(int)d->PC->carry.size()){
    object* obj = d->PC->carry[slot];
    std::vector<object*> &carry = d->PC->carry;
    object** selected_slot = 0;
    switch((int)obj->get_type()){
    case(objtype_WEAPON):
      selected_slot = &(d->PC->weapon_slot);
      break;
    case(objtype_OFFHAND):
      selected_slot = &(d->PC->offhand_slot);
      break;
    case(objtype_RANGED):
      selected_slot = &(d->PC->ranged_slot);
      break;
    case(objtype_ARMOR):
      selected_slot = &(d->PC->armor_slot);
      break;
    case(objtype_HELMET):
      selected_slot = &(d->PC->helmet_slot);
      break;
    case(objtype_CLOAK):
      selected_slot = &(d->PC->cloak_slot);
      break;
    case(objtype_GLOVES):
      selected_slot = &(d->PC->gloves_slot);
      break;
    case(objtype_BOOTS):
      selected_slot = &(d->PC->boots_slot);
      break;
    case(objtype_AMULET):
      selected_slot = &(d->PC->amulet_slot);
      break;
    case(objtype_LIGHT):
      selected_slot = &(d->PC->light_slot);
      break;
    case(objtype_RING):
      if(!d->PC->ring_one_slot){
        selected_slot = &(d->PC->ring_one_slot);
      }else{
        selected_slot = &(d->PC->ring_two_slot);
      }
      break;
    default:
      io_queue_message("Item cannot be equipped.");
    }
    if(selected_slot){
      io_queue_message("%s now equipped",obj->get_name());
      if(*selected_slot){
        d->PC->carry.push_back(*selected_slot);
      }
      *selected_slot = obj;
      carry.erase(carry.begin()+slot);
    }

  }
}

void pc_take_off(dungeon *d,int slot){
  pc *PC = d->PC;
  object* obj = 0;
  switch(slot){
  case 0://a
    obj = PC->weapon_slot;
    PC->weapon_slot = 0;
    break;
  case 1://b
    obj = PC->offhand_slot;
    PC->offhand_slot = 0;
    break;
  case 2://c
    obj = PC->ranged_slot;
    PC->ranged_slot = 0;
    break;
  case 3://d
    obj = PC->armor_slot;
    PC->armor_slot = 0;
    break;
  case 4://e
    obj = PC->helmet_slot;
    PC->helmet_slot = 0;
    break;
  case 5://f
    obj = PC->cloak_slot;
    PC->cloak_slot = 0;
    break;
  case 6://g
    obj = PC->gloves_slot;
    PC->gloves_slot = 0;
    break;
  case 7://h
    obj = PC->boots_slot;
    PC->boots_slot = 0;
    break;
  case 8://i
    obj = PC->amulet_slot;
    PC->amulet_slot = 0;
    break;
  case 9://j
    obj = PC->light_slot;
    PC->light_slot = 0;
    break;
  case 10://k
    obj = PC->ring_one_slot;
    PC->ring_one_slot = 0;
    break;
  case 11://l
    obj = PC->ring_two_slot;
    PC->ring_two_slot = 0;
    break;
 }
  if(obj){
    PC->carry.push_back(obj);
    io_queue_message("Took off %s, much better",obj->get_name());
  }

}
