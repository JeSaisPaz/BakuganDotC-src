// bdc 0x088fe12c BtlDemoCamResetKeysNear
#include "bdc.h"

/* Sets the clip range of the battle demo camera (`BtlDemoCam`, `BtlDemoCamCtor`) to near 0.5
   / far 100 (close-up shots) and clears its key tracks (`BtlDemoCamResetKeys`, which also stores
   the caller's live-in VFPU column C720 into the vector keys; this function does not touch it). */
void BtlDemoCamResetKeysNear(BtlDemoCam *self)
{
    self->base.nearZ = 0.5f;
    self->base.farZ = 100.0f;
    BtlDemoCamResetKeys(self);
}
