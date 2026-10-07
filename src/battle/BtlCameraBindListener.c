// bdc 0x08847fb4 BtlCameraBindListener
#include "bdc.h"

/* Re-binds the 3D sound listener (`SndGetListener`/`SndListenerBind`) to the camera position
   `+0x2f0`, passed as both bind arguments. */
void BtlCameraBindListener(void *camera)
{
    float *pos = ((BtlCamera *)camera)->listenerPos;

    SndListenerBind(SndGetListener(), pos, pos);
}
