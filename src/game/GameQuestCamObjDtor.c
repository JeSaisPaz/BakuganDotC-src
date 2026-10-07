// bdc 0x088fc36c GameQuestCamObjDtor
#include "bdc.h"

/* Root destructor of the quest camera object hierarchy: installs the root vtable
   `g_gameQuestCamObjRootVtbl` and frees the object (under `MemLock`) when bit 0 of `flags` is set. */

void GameQuestCamObjDtor(void *obj, u32 flags)
{
  if (obj != (void *)0x0) {
    *(VtblEntry **)obj = g_gameQuestCamObjRootVtbl;
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
