// bdc 0x089ab95c UiPauseSettingsDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiPauseSettings screen (task id 301): reinstalls vtable
   `g_uiPauseSettingsVtbl`, waits for the GE, resets the frame mode, restores the pad's stick-as-d-pad byte,
   stores its task id in `g_lastScreenTaskId` (last closed screen); then `UiScreenDtor``(this, 0)` and
   frees the object when `flags & 1`. */

void UiPauseSettingsDtor(UiPauseSettings *self, u32 flags)

{
  if (self != (UiPauseSettings *)0x0) {
    (self->base).base.vtable = g_uiPauseSettingsVtbl;
    GfxWaitGeIdle();
    UiScreenSetFrameMode((CoreTask *)self,1);
    ((self->base).pad)->stickEmulatesDpad = self->savedStickEmulatesDpad;
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

