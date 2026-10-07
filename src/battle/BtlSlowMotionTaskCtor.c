// bdc 0x08849260 BtlSlowMotionTaskCtor
#include "bdc.h"

/* Constructor of the slow-motion task: base task init, slow-motion vtable, the embedded camera
   (constructed, the triggering unit, its target and the third creation argument stored, defaults reset, then near 20, far 35000,
   field of view 50 and a full camera update with all flags), and `started`/`reserved2bc` cleared.
   Returns `task`. */

CoreTask *BtlSlowMotionTaskCtor(CoreTask *task, BtlBakugan *unit, void *target, u32 argC)
{
    BtlSlowMotionTask *self = (BtlSlowMotionTask *)task;

    CoreTaskInit(&self->base);
    self->base.vtable = g_btlSlowMotionTaskVtbl;
    GfxCameraCtor(&self->camera.base);
    self->unit = unit;
    self->target = target;
    self->argC = argC;
    GfxCameraInit(&self->camera);
    self->camera.nearZ = 20.0f;
    self->camera.farZ = 35000.0f;
    self->camera.fov = 50.0f;
    GfxCameraUpdate(&self->camera, 0xffffffff);
    self->started = 0;
    self->reserved2bc = NULL;
    return &self->base;
}
