// bdc 0x08a2c78c GameQuestCamTargetDtor
#include "bdc.h"

/* Destructor (vtable `0x08af6e58` entry 1) of the quest camera spring point with a look-at
   (`GameQuestCamTargetCtor`): reinstalls its vtable, runs `GameQuestCamObjDtor` and frees the
   object when `flags & 1`. */

void GameQuestCamTargetDtor(GameQuestCamTarget *self, u32 flags)

{
  if (self != (GameQuestCamTarget *)0x0) {
    self->base.vtbl = g_gameQuestCamTargetVtbl;
    GameQuestCamObjDtor(self,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

