#include "object.h"
#include "utils.h"
#include "dims.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static uint16_t max_object_cells(dungeon *d)
{
  uint32_t i;
  uint16_t sum;

  for (i = sum = 0; i < d->num_rooms; i++) {
    sum += d->rooms[i].size[dim_y] * d->rooms[i].size[dim_x];
  }

  return sum;
}
void gen_objects(dungeon *d){
  uint16_t i;
  object o;
  uint32_t room;
  uint16_t num_objects = std::min(d->max_objects, max_object_cells(d));

  std::cerr<<"gen_objects"<<std::endl;
  for(i = 0; i < num_objects; i++){
    bool need_description = true;
    object_description *desc;

    while(need_description){
      desc = &(d->object_descriptions[std::rand()%d->object_descriptions.size()]);
      if(desc->artifact){
        for(size_t i = 0; i < d->picked_objects.size();i++){
          if(!(d->picked_objects[i].compare(desc->name))){
            break;
          }
        }
      }
      if((uint32_t)std::rand()%100>desc->rarity){
        break;
      }
      need_description = 0;
    }
    o = (desc)->create_object();
    pair_t p;
    do {
      room = rand_range(1, d->num_rooms - 1);
      p[dim_y] = rand_range(d->rooms[room].position[dim_y],
                            (d->rooms[room].position[dim_y] +
                             d->rooms[room].size[dim_y] - 1));
      p[dim_x] = rand_range(d->rooms[room].position[dim_x],
                            (d->rooms[room].position[dim_x] +
                             d->rooms[room].size[dim_x] - 1));
    } while (d->object_map[p[dim_y]][p[dim_x]]);
    d->object_map[p[dim_y]][p[dim_x]] = &o;
  }
}
char object_get_symbol(const object *o){
  return object_symbol[o->type];
  
}
