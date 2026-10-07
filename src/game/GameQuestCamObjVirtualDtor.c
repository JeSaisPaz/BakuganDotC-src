// bdc 0x08a2ca64 GameQuestCamObjVirtualDtor
#include "bdc.h"

/* Virtual destructor of the quest camera object root class (vtable `0x08af6ee0` entry 1):
   reinstalls the root vtable and frees the object when `flags & 1`. */

void GameQuestCamObjVirtualDtor(void *obj, u32 flags)

{
  if ((obj != (void *)0x0) && (*(VtblEntry **)obj = g_gameQuestCamObjRootVtbl, (flags & 1) != 0)) {
    MemLock();
    MemFree(obj,(const char *)0,0);
    MemUnlock();
  }
  return;
}

