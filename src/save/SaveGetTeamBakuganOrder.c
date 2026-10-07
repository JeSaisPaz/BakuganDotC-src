// bdc 0x0880dff4 SaveGetTeamBakuganOrder
#include "bdc.h"

/* Maps the Bakugan in team slot `slot` (`SaveGetTeamBakugan`) to its 0-based order through the
   21-entry table `0x08a3421c` (a permutation of 0..19); 0 for an id outside 1..20. The first
   argument is unused. */

s32 SaveGetTeamBakuganOrder(void *unused, s32 slot)

{
  s32 id;
  s32 order;
  
  id = SaveGetTeamBakugan(slot);
  order = 0;
  if ((0 < id) && (id < 0x15)) {
    order = g_teamBakuganOrderTable[id];
  }
  return order;
}

