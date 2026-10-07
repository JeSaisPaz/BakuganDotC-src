// bdc 0x08857584 ActorCrystalGetShakeOffset
#include "bdc.h"

/* Returns entry `frame & 31` of the 32-entry s16 hit-shake table `0x08aba7ac`
   (`ActorCrystalUpdateLogic`). */

int ActorCrystalGetShakeOffset(ActorCrystal *self, u32 frame)

{
  short table [32];
  
  memcpy(table,g_actorCrystalShakeTable,0x40);
  return (int)table[frame & 0x1f];
}

