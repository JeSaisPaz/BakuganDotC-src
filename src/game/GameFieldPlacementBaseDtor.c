// bdc 0x088f4cf8 GameFieldPlacementBaseDtor
#include "bdc.h"

/* Destructor of the placement record base (vtable `0x08af43b4` slot 1): frees it when `flags & 1`.
    */

void GameFieldPlacementBaseDtor(void *rec, u32 flags)
{
  if (rec != NULL) {
    ((GameFieldPlacementBase *)rec)->vtbl = g_gameFieldPlacementBaseVtbl;
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(rec, NULL, 0);
      MemUnlock();
    }
  }
}
