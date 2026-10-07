// bdc 0x088f9104 GameQuestCamRailSpringDtor
#include "bdc.h"

/* Destructor (slot 1) of the quest camera class with vtable `0x08af4504`: restores the parent
   vtable and runs the parent destructor (the spring destructor `GameQuestCamObjDtor`), freeing the object
   when `flags & 1`. */

void GameQuestCamRailSpringDtor(void *obj, u32 flags)

{
  if (obj != (void *)0x0) {
    ((GameQuestCamSpring *)obj)->vtbl = g_gameQuestCamSpringBaseVtbl;
    GameQuestCamObjDtor(obj,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

