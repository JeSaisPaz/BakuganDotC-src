// bdc 0x08848f54 BtlFinishTaskStart
#include "bdc.h"

/* Creates the end-of-battle cinematic task (id 0x14a = 330, 0x310 bytes from the low heap,
   `BtlFinishTaskCtor`) for `unit`. With `mode == 0` the mode becomes 2 when the unit's virtual
   slot 11 returns non-zero. Opens UI window 2, pauses the battle main task (task 100, flag 1),
   stores it in the new task's `mainTask` and inserts the task at priority 100
   (`CoreTaskInsert`); with a non-zero mode refreshes the scene (`BtlMainUpdateScene`). Ends
   the cut-in (`BtlEndCutIn`), sets the main task's `flashTarget` to 0.85, slows motion time to
   0.25 (`GfxSetMotionTimeScale`), shows the HUD again when the mode is still 0
   (`g_btlHudHidden`), binds the sound listener to the unit's position (`SndListenerBind`) and
   plays sound 0x200133 when a sound manager exists. Returns the task (the id is written even when
   the allocation failed, as compiled). */
void *BtlFinishTaskStart(void *unit, int arg, int mode)
{
    GfxModel *model = (GfxModel *)unit;
    BtlMain *main = (BtlMain *)CoreTaskFind(100);
    BtlFinishTask *alloc;
    BtlFinishTask *task = NULL;
    BtlMain *camera;
    bool fromLow;

    if (mode == 0) {
        const VtblEntry *entry = &((const VtblEntry *)model->base.vtable)[11];

        if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
            mode = 2;
        }
    }
    UiSetWindowActive(2, 1);
    CoreTaskSetFlags(&main->base, 1);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    alloc = (BtlFinishTask *)MemAlloc(sizeof(BtlFinishTask), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (alloc != NULL) {
        BtlFinishTaskCtor(alloc, unit, arg, mode);
        task = alloc;
    }
    task->base.id = 0x14a;
    task->mainTask = main;
    CoreTaskInsert(&task->base, 100);
    if (mode != 0) {
        BtlMainUpdateScene(main);
    }
    BtlGetCameraTask();
    BtlEndCutIn();
    camera = (BtlMain *)BtlGetCameraTask();
    camera->flashTarget = 0.850000024f;
    GfxSetMotionTimeScale(0.25f);
    if (mode == 0) {
        g_btlHudHidden = 0;
    }
    SndListenerBind(SndGetListener(), model->pos, model->pos);
    if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0x200133, 0, 0);
    }
    return task;
}
