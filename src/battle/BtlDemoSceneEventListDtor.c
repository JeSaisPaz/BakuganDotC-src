// bdc 0x08908548 BtlDemoSceneEventListDtor
#include "bdc.h"

/* Destructor of the scene's event table list head: clears it (`CoreObjectListDeleteAll`) and
   frees it when bit 0 of `flags` is set. Does nothing for NULL. */
void BtlDemoSceneEventListDtor(void *list, u32 flags)
{
  if (list != NULL) {
    CoreObjectListDeleteAll(list);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(list, NULL, 0);
      MemUnlock();
    }
  }
}
