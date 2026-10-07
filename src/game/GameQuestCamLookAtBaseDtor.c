// bdc 0x08a2c820 GameQuestCamLookAtBaseDtor
#include "bdc.h"

/* Destructor (vtable `0x08af6e98` entry 1) of the intermediate look-at base of the quest camera
   springs: reinstalls that vtable, runs `GameQuestCamObjDtor` and frees the object when `flags &
   1`. */

void GameQuestCamLookAtBaseDtor(void *spring, u32 flags)

{
  if (spring != (void *)0x0) {
    ((GameQuestCamSpring *)spring)->vtbl = g_gameQuestCamSpringBaseVtbl;
    GameQuestCamObjDtor(spring,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(spring,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

