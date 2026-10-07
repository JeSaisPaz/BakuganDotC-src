// bdc 0x08855610 ActorCrystalRandomStyle
#include "bdc.h"

/* Picks a random crystal style id: indexes the six-entry table `{0, 2, 3, 4, 5, 6}` with
   `CoreRandNext(6)` and returns the entry (note that style 1 is never produced). The result is the
   `style` argument of `ActorCrystalSetStyle`. */

u32 ActorCrystalRandomStyle(ActorCrystal *self)

{
  u32 table[6];

  table[0] = 0;
  table[1] = 2;
  table[2] = 3;
  table[3] = 4;
  table[4] = 5;
  table[5] = 6;
  return table[CoreRandNext(6)];
}

