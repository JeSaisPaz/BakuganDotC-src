// bdc 0x088fe0f8 BtlDemoCamResetKeysFar
#include "bdc.h"

/* Sets the clip range of the battle demo camera (`BtlDemoCam`, whose base is a `GfxCamera`) to
   near 20 / far 35000 and then resets its key tracks (`BtlDemoCamResetKeys`). */
void BtlDemoCamResetKeysFar(BtlDemoCam *self)
{
  self->base.nearZ = 20.0f;
  self->base.farZ = 35000.0f;
  BtlDemoCamResetKeys(self);
}
