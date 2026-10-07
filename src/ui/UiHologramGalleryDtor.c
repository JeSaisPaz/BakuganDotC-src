// bdc 0x0891c238 UiHologramGalleryDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the hologram gallery screen (task id 391): restores its vtable,
   waits for the GE, sets frame mode 1, clears pad->stickEmulatesDpad and stores its task id in
   g_lastScreenTaskId. Then UiScreenDtor(screen, 0); frees the object when flags & 1.
   It frees no sprites or models of its own. */

void UiHologramGalleryDtor(UiHologramGallery *self, u32 flags)
{
  if (self != NULL) {
    self->base.base.vtable = &g_uiHologramGalleryVtable;
    GfxWaitGeIdle();
    UiScreenSetFrameMode(&self->base.base, 1);
    self->base.pad->stickEmulatesDpad = 0;
    g_lastScreenTaskId = self->base.base.id;
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, NULL, 0);
      MemUnlock();
    }
  }
}
