// bdc 0x08937efc UiUnlockResultDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the unlock result screen (task id 375): reinstalls
   `g_uiUnlockResultVtbl`, waits for the GE, re-enables the update of task 500 if it exists,
   resets the frame mode to 1, restores the pad's stick-as-d-pad byte, deletes both text printers
   through their virtual destructors (flag 3) and stores its task id in `g_lastScreenTaskId`.
   When a reward model exists it is queued for deletion (`CoreObjectDeferDelete`), its motion
   freed (`UiUnlockResultFreeModelMotion`), the reward camera deleted (virtual destructor, flag 3)
   and `g_gfxActiveCamera` restored from `savedCamera` if that is non-null. Clears profile word
   0x34, then `UiScreenDtor``(screen, 0)`; frees the object when `flags & 1`. NULL `self`: no-op. */

void UiUnlockResultDtor(UiUnlockResult *self, u32 flags)
{
    CoreTask *task;
    const VtblEntry *entry;
    s32 i;

    if (self == NULL) {
        return;
    }
    self->base.base.vtable = g_uiUnlockResultVtbl;
    GfxWaitGeIdle();
    task = CoreTaskFind(500);
    if (task != NULL) {
        CoreTaskClearFlags(task, 1);
    }
    UiScreenSetFrameMode((CoreTask *)self, 1);
    self->base.pad->stickEmulatesDpad = self->savedStickEmulatesDpad;
    for (i = 0; i < 2; i++) {
        if (self->printers[i] != NULL) {
            entry = &self->printers[i]->layer.vtbl[1];
            ((void (*)(void *, s32))entry->fn)((u8 *)self->printers[i] + entry->delta, 3);
            self->printers[i] = NULL;
        }
    }
    g_lastScreenTaskId = self->base.base.id;
    if (self->model != NULL) {
        CoreObjectDeferDelete((CoreObject *)self->model, 0);
        self->model = NULL;
        UiUnlockResultFreeModelMotion(self);
        if (self->camera != NULL) {
            entry = &((const VtblEntry *)((CoreNode *)self->camera)->vtable)[1];
            ((void (*)(void *, s32))entry->fn)((u8 *)self->camera + entry->delta, 3);
            self->camera = NULL;
            self->camera = NULL; /* the binary stores 0 twice */
        }
        if (self->savedCamera != NULL) {
            g_gfxActiveCamera = self->savedCamera;
        }
    }
    SaveProfileSetWord(SaveGetProfile(), 0x34, 0);
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
