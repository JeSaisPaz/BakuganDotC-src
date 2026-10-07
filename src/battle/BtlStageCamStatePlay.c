// bdc 0x089045c8 BtlStageCamStatePlay
#include "bdc.h"

/* State 1 of the stage camera demo task: START (pad pressed bit 3) sets the
   skip flag; once skipped, advances the state and returns. Otherwise plays the
   script keys, runs the demo camera's update (vtable slot 2) and rebuilds its
   view (GfxCameraUpdate flags 2). */
void BtlStageCamStatePlay(CoreTask *task)
{
    BtlStageCam *self = (BtlStageCam *)task;
    const VtblEntry *update;

    if (g_padState != NULL && (g_padState->pressed & 8) != 0) {
        self->skipped = 1;
    }
    if (self->skipped != 0) {
        self->state++;
        return;
    }
    BtlStageCamPlayKeys(self);
    update = &((const VtblEntry *)self->cam->base.base.vtable)[2];
    ((void (*)(void *))update->fn)((u8 *)self->cam + update->delta);
    GfxCameraUpdate(&self->cam->base, 2);
}
