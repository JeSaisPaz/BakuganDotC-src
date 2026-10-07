// bdc 0x08a29f88 ActorCrystalIsUntargetable
#include "bdc.h"

/* Crystal override of the battle-unit virtual slot 17 (`+0x8c`, base `0x08a29fa8` tests `+0x476 |
   +0x59c`): returns 1 when the crystal has its stand object `+0x6d0` or the byte `+0x59c` is set,
   else 0. */

int ActorCrystalIsUntargetable(ActorCrystal *self)
{
  return self->base.untargetable != 0 || self->auxObject != NULL;
}
