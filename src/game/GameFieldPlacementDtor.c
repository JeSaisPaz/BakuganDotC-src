// bdc 0x088f4ea4 GameFieldPlacementDtor
#include "bdc.h"

/* Destructor of the placement record (vtable `0x08af43c4` slot 1): runs the base destructor and
   frees it when `flags & 1`. */

void GameFieldPlacementDtor(void *rec, u32 flags)

{
  if (rec != (void *)0x0) {
    ((GameFieldPlacement *)rec)->vtbl = g_gameFieldPlacementVtbl;
    GameFieldPlacementBaseDtor(rec,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(rec,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

