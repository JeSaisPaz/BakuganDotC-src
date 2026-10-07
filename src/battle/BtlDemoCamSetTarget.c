// bdc 0x088fde94 BtlDemoCamSetTarget
#include "bdc.h"

/* Sets the object followed by the battle demo camera (a `GfxCamera` embedded at demo task `+0x100`,
   vtable `0x08af45e4` at `+0x20`, BtlDemoCamCtor) (`+0x2a0`). */
void BtlDemoCamSetTarget(BtlDemoCam *cam, void *target)
{
    cam->target = target;
}
