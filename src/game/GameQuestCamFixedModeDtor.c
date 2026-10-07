// bdc 0x088f6744 GameQuestCamFixedModeDtor
#include "bdc.h"

/* Destructor (slot 1) of the quest camera class with vtable `0x08af43d4`: restores the parent
   vtable and runs the parent destructor (the spring destructor `GameQuestCamObjDtor`), freeing the object
   when `flags & 1`. */

void GameQuestCamFixedModeDtor(GameQuestCamFixedMode *self, u32 flags)

{
  if (self != (GameQuestCamFixedMode *)0x0) {
    self->base.base.vtbl = g_gameQuestCamTargetVtbl;
    GameQuestCamObjDtor(self,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

