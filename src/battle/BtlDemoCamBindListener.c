// bdc 0x088fe190 BtlDemoCamBindListener
#include "bdc.h"

/* Binds the 3D sound listener (SndGetListener, SndListenerBind) to the battle demo
   camera's listener point `self->listener` (+0x330), used as both the listener
   position and its target. */
void BtlDemoCamBindListener(BtlDemoCam *self)
{
    SndListenerBind(SndGetListener(), self->listener, self->listener);
}
