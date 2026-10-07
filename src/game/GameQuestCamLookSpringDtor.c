// bdc 0x088f8c94 GameQuestCamLookSpringDtor
#include "bdc.h"

/* Destructor (slot 1) of the quest camera class with vtable `0x08af44cc`: restores the parent
   vtable and runs the parent destructor (`GameQuestCamObjDtor`), freeing the object when `flags &
   1`. */

void GameQuestCamLookSpringDtor(GameQuestCamLookSpring *self, u32 flags)

{
  if (self != (GameQuestCamLookSpring *)0x0) {
    (self->base).vtbl = g_gameQuestCamSpringBaseVtbl;
    GameQuestCamObjDtor(self,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

