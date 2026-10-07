// bdc 0x089ee1a0 UiSpriteMngGet
#include "bdc.h"

/* Returns the sprite manager task (`CoreTaskFind(0x276a)`). The manager keeps a package/layer at
   `+0x10`, a table of 0x20 sprite pointers at `+0x14`, a global visible flag at `+0x18` and the
   sprite count at `+0x1c`. */

void *UiSpriteMngGet(void)

{
  void *task;
  
  task = CoreTaskFind(0x276a);
  return task;
}

