// bdc 0x088597d8 ActorCrystalFireType4
#include "bdc.h"

/* Entry 4 of the crystal fire-type table `0x08a67104`: calls `ActorCrystalMode4Update`. */

int ActorCrystalFireType4(ActorCrystal *self)
{
  return ActorCrystalMode4Update(self);
}
