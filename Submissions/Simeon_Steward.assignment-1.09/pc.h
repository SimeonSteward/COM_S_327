#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "object.h"
# include "dims.h"
# include "character.h"
# include "dungeon.h"

# define PC_CAPACITY 10

class pc : public character {
 public:
  ~pc() {}
  terrain_type known_terrain[DUNGEON_Y][DUNGEON_X];
  uint8_t visible[DUNGEON_Y][DUNGEON_X];
  object* weapon_slot;
  object* offhand_slot;
  object* ranged_slot;
  object* armor_slot;
  object* helmet_slot;
  object* cloak_slot;
  object* gloves_slot;
  object* boots_slot;
  object* amulet_slot;
  object* light_slot;
  object* ring_one_slot;
  object* ring_two_slot;
  std::vector<object*> carry;
  pc();
  uint32_t get_damage();
};


void pc_delete(pc *pc);
uint32_t pc_is_alive(dungeon *d);
void config_pc(dungeon *d);
uint32_t pc_next_pos(dungeon *d, pair_t dir);
void place_pc(dungeon *d);
uint32_t pc_in_room(dungeon *d, uint32_t room);
void pc_learn_terrain(pc *p, pair_t pos, terrain_type ter);
terrain_type pc_learned_terrain(pc *p, int16_t y, int16_t x);
void pc_init_known_terrain(pc *p);
void pc_observe_terrain(pc *p, dungeon *d);
int32_t is_illuminated(pc *p, int16_t y, int16_t x);
void pc_reset_visibility(pc *p);
void pc_wear(dungeon *d,int slot);
void pc_drop(dungeon *d,int slot);
void pc_take_off(dungeon *d,int slot);
void pc_expunge(dungeon *d,int slot);
#endif
