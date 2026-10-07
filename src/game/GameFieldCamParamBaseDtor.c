// bdc 0x08a2c0a8 GameFieldCamParamBaseDtor
#include "bdc.h"

/* Destructor of the base class of the field camera's quest parameter object (vtable `0x08af6dc0` at
   `+4`: entry 1 this destructor, entry 2 pure virtual, entry 3 `GameFieldCamParamGet`):
   reinstalls the base vtable and frees the object when `flags & 1`. */

void GameFieldCamParamBaseDtor(void *obj, u32 flags)
{
  if (obj != NULL) {
    ((GameFieldCamParam *)obj)->vtbl = (const GameFieldCamParamVtbl *)g_gameFieldCamParamBaseVtbl;
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj, NULL, 0);
      MemUnlock();
    }
  }
}
