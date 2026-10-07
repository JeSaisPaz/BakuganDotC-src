// bdc 0x08a2c104 GameFieldCamParamDtor
#include "bdc.h"

/* Destructor of the field camera's quest parameter object (0x20 bytes, vtable `0x08af6de0` at `+4`,
   stored at field camera `+0x5c8` by `GameFieldCameraLoadQuestCam`): installs the base vtable
   `0x08af6dc0` (the base destructor `GameFieldCamParamBaseDtor` is inlined) and frees the object
   when `flags & 1`. */

void GameFieldCamParamDtor(void *obj, u32 flags)
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
