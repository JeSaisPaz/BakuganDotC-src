// bdc 0x088fda90 GameQuestCamCtrlBaseDtor
#include "bdc.h"

/* Base destructor of the quest-field camera controller: restores vtable `0x08af45cc` at `+0x2c` and
   frees the object (under `MemLock`) when bit 0 of `flags` is set. */

void GameQuestCamCtrlBaseDtor(GameQuestCamCtrl *self, u32 flags)

{
  if ((self != (GameQuestCamCtrl *)0x0) &&
     (self->vtbl = g_questCamCtrlBaseVtbl, (flags & 1) != 0)) {
    MemLock();
    MemFree(self, (const char *)0, 0);
    MemUnlock();
  }
  return;
}

