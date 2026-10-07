// bdc 0x08904544 BtlStageCamStateStart
#include "bdc.h"

/* State 0 of the stage camera demo task (task id 0x6b, 0x58 bytes, vtable `0x08af466c`,
   `BtlStageCamCtor`): plays the first frame of the script keys (`BtlStageCamPlayKeys`), makes
   the task's demo camera the active camera (`g_gfxActiveCamera`) while saving the previous one in
   `savedCam`, binds the sound listener to it (`BtlDemoCamBindListener`), calls the camera's
   virtual slot 2 twice (update) and advances `state`. */
void BtlStageCamStateStart(void *task)
{
    BtlStageCam *self = (BtlStageCam *)task;
    GfxCamera *prev;
    const VtblEntry *entry;

    BtlStageCamPlayKeys(self);
    prev = g_gfxActiveCamera;
    g_gfxActiveCamera = &self->cam->base;
    self->savedCam = prev;
    BtlDemoCamBindListener(self->cam);
    entry = &((const VtblEntry *)self->cam->base.base.vtable)[2];
    ((void (*)(void *))entry->fn)((u8 *)self->cam + entry->delta);
    entry = &((const VtblEntry *)self->cam->base.base.vtable)[2];
    ((void (*)(void *))entry->fn)((u8 *)self->cam + entry->delta);
    self->state++;
}
