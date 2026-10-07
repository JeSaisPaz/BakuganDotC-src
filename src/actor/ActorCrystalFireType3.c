// bdc 0x088597bc ActorCrystalFireType3
#include "bdc.h"

/* Entry 3 of the crystal fire-type table `0x08a67104`: calls `ActorCrystalMode4Update`. */

int ActorCrystalFireType3(ActorCrystal *self)
{
  return ActorCrystalMode4Update(self);
}
