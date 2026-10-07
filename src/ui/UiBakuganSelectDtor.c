// bdc 0x0892b5ac UiBakuganSelectDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiBakuganSelect screen (task id 371): reinstalls vtable
   `g_uiBakuganSelectVtbl`, waits for the GE, resets the frame mode, runs `UiBakuganSelectFreeModels`,
   frees all GMO motions, deletes the camera `+0x1cf4` through its virtual destructor, stores its task id
   in `g_lastScreenTaskId` (last closed screen); then `UiScreenDtor``(this, 0)` and frees the object
   when `flags & 1`. */

void UiBakuganSelectDtor(UiBakuganSelect *self, u32 flags)

{
  if (self != (UiBakuganSelect *)0x0) {
    (self->base).base.vtable = g_uiBakuganSelectVtbl;
    GfxWaitGeIdle();
    UiScreenSetFrameMode((CoreTask *)self, 1);
    UiBakuganSelectFreeModels(self);
    GmoMotionFreeAll(GmoMotionMgrGet(), 0);
    if (self->camera != (void *)0x0) {
      const VtblEntry *entry = (const VtblEntry *)((CoreNode *)self->camera)->vtable + 1;

      ((void (*)(void *, int))entry->fn)((char *)self->camera + entry->delta, 3);
      self->camera = (void *)0x0;
    }
    g_lastScreenTaskId = (self->base).base.id;
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
