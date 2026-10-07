// bdc 0x088f8938 GameQuestCamEyeSpringDtor
#include "bdc.h"

/* Destructor (slot 1) of the quest camera class with vtable `0x08af4494`: restores the parent
   vtable and runs the parent destructor (the spring destructor `GameQuestCamObjDtor`), freeing the object
   when `flags & 1`. */

void GameQuestCamEyeSpringDtor(GameQuestCamEyeSpring *self, u32 flags)

{
  if (self != (GameQuestCamEyeSpring *)0x0) {
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

