// bdc 0x088fe15c BtlDemoCamResetKeysFarB
#include "bdc.h"

/* Sets the demo camera's clip planes to near 20 / far 35000 and resets its keys
   (`BtlDemoCamResetKeys`); the same body as `BtlDemoCamResetKeysFar`, a separate copy used by
   `BtlAppearDemoStateLoad`. */
void BtlDemoCamResetKeysFarB(BtlDemoCam *self)
{
    self->base.nearZ = 20.0f;
    self->base.farZ = 35000.0f;
    BtlDemoCamResetKeys(self);
}
