// bdc 0x08903700 BtlStageCamCtor
#include "bdc.h"

/* Constructor of the stage camera demo task (`BtlStageCam`, task id 0x6b, 0x58 bytes):
   `CoreTaskInit`, installs `g_btlStageCamVtbl`, clears the state fields (`eyeTrack` = 1),
   stores the script index `index` and the followed unit `unit`; allocates the 0x1a4-byte camera
   scene from the low heap end (`MemAlloc` under `MemLock`, `BtlStageCamSceneCtor`) and loads
   script `index` into it (`BtlStageCamSceneLoad`, even when the allocation failed); allocates a
   0x3c0-byte demo camera the same way (`BtlDemoCamCtor`) following `unit` (or the player's
   Bakugan, `BtlGetPlayerBakugan`) with `BtlDemoCamResetKeysFar` and near plane 30; scripts
   below 0x28 get shot id 999 and `farFlag` 0, the others 998 and `farFlag` 1; sets the camera's
   `fixedOffset` and clears `savedCam`. Returns `task`. */

CoreTask *BtlStageCamCtor(CoreTask *task, u32 index, void *unit)
{
    BtlStageCam *self = (BtlStageCam *)task;
    BtlStageCamScene *scene;
    BtlDemoCam *cam;
    bool fromLow;

    CoreTaskInit(task);
    self->base.vtable = g_btlStageCamVtbl;
    self->state = 0;
    self->step = 0;
    self->frame = 0;
    self->keyPhase = 0;
    self->eyeCursor = 0;
    self->keyBCursor = 0;
    self->lensCursor = 0;
    self->resv38 = 0;
    self->resv3c = 0;
    self->resv4c = 0;
    self->skipped = 0;
    self->scene = NULL;
    self->resv40 = 0;
    self->eyeTrack = 1;
    self->keyBTrack = 0;
    self->index = (s32)index;
    self->unit = (GfxModel *)unit;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    scene = (BtlStageCamScene *)MemAlloc(0x1a4, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (scene != NULL) {
        BtlStageCamSceneCtor(scene);
    }
    self->scene = scene;
    BtlStageCamSceneLoad(scene, self->index);

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    cam = (BtlDemoCam *)MemAlloc(0x3c0, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (cam != NULL) {
        BtlDemoCamCtor(cam);
    }
    self->cam = cam;

    if (unit == NULL) {
        unit = BtlGetPlayerBakugan();
    }
    BtlDemoCamSetTarget(self->cam, unit);
    BtlDemoCamResetKeysFar(self->cam);
    self->cam->base.nearZ = 30.0f;
    if (self->index < 0x28) {
        self->cam->shotId = 999;
        self->farFlag = 0;
    } else {
        self->cam->shotId = 998;
        self->farFlag = 1;
    }
    self->cam->fixedOffset = 1;
    self->savedCam = NULL;
    return task;
}
