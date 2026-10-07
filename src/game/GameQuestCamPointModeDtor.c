// bdc 0x088f81dc GameQuestCamPointModeDtor
#include "bdc.h"

/* Destructor of the point camera mode (vtable `0x08af4454` slot 1): mode base destructor
   (`GameQuestCamModeBaseDtor`) and free when `flags & 1`. */

void GameQuestCamPointModeDtor(GameQuestCamPointMode *self, u32 flags)

{
  if (self != (GameQuestCamPointMode *)0x0) {
    (self->base).base.base.vtbl = g_gameQuestCamPointVtbl;
    GameQuestCamModeBaseDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

