#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "dice.h"

typedef enum object_type
  {
   objtype_no_type,
   objtype_WEAPON,
   objtype_OFFHAND,
   objtype_RANGED,
   objtype_LIGHT,
   objtype_ARMOR,
   objtype_HELMET,
   objtype_CLOAK,
   objtype_GLOVES,
   objtype_BOOTS,
   objtype_AMULET,
   objtype_RING,
   objtype_SCROLL,
   objtype_BOOK,
   objtype_FLASK,
   objtype_GOLD,
   objtype_AMMUNITION,
   objtype_FOOD,
   objtype_WAND,
   objtype_CONTAINER
  } object_type_t;

const char object_symbol[] =
  {
   '*', /* objtype_no_type */
   '|', /* objtype_WEAPON */
   ')', /* objtype_OFFHAND */
   '}', /* objtype_RANGED */
   '~', /* objtype_LIGHT */
   '[', /* objtype_ARMOR */
   ']', /* objtype_HELMET */
   '(', /* objtype_CLOAK */
   '{', /* objtype_GLOVES */
   '\\', /* objtype_BOOTS */
   '"', /* objtype_AMULET */
   '=', /* objtype_RING */
   '`', /* objtype_SCROLL */
   '?', /* objtype_BOOK */
   '!', /* objtype_FLASK */
   '$', /* objtype_GOLD */
   '/', /* objtype_AMMUNITION */
   ',', /* objtype_FOOD */
   '-', /* objtype_WAND */
   '%', /* objtype_CONTAINER */
  };

class object{
public:
  std::string name;
  std::string desc;
  object_type_t type;
  uint32_t color;
  uint32_t hit, dodge, defence, weight, speed, attribute, value;
  dice damage;
  bool artifact;
  uint32_t rarity;


};
char object_get_symbol(const object  *o);
#include "dungeon.h"
void gen_objects(dungeon *d);
# endif
