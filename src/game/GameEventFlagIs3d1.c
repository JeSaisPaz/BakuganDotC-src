// bdc 0x088f554c GameEventFlagIs3d1
#include "bdc.h"

/* True when `id` is 0x3d1 (the throw-unlock flag tested by `GameEventIsFlag3d1Set`). */

bool GameEventFlagIs3d1(s16 id)

{
  return id == 0x3d1;
}

