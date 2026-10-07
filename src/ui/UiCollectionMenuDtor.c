// bdc 0x089737c4 UiCollectionMenuDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiCollectionMenu screen (task id 311): reinstalls vtable
   `g_uiCollectionMenuVtbl`, waits for the GE, runs `UiCollectionMenuFreeItemBox`, restores the pad's
   stick-as-d-pad byte, frees all GMO motions, deletes the camera through its deleting destructor
   (vtable entry 1, flag 3) when present, stores its task id in `g_lastScreenTaskId` (last closed
   screen); then `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiCollectionMenuDtor(UiCollectionMenu *self, u32 flags)

{
  GfxCamera *camera;

  if (self != (UiCollectionMenu *)0x0) {
    (self->base).base.vtable = g_uiCollectionMenuVtbl;
    GfxWaitGeIdle();
    UiCollectionMenuFreeItemBox(self);
    ((self->base).pad)->stickEmulatesDpad = self->savedStickEmulatesDpad;
    GmoMotionFreeAll(GmoMotionMgrGet(),0);
    camera = self->camera;
    if (camera != NULL) {
      const VtblEntry *dtor = &((const VtblEntry *)camera->base.vtable)[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)camera + dtor->delta, 3);
      self->camera = NULL;
    }
    g_lastScreenTaskId = (self->base).base.id;
    UiScreenDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}
