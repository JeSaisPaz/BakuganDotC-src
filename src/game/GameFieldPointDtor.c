// bdc 0x08a2c2f8 GameFieldPointDtor
#include "bdc.h"

/* Destructor (vtable `0x08af6e00` entry 1) of the 0x40-byte field point object built by
   `GameFieldPointCtor`: reinstalls the vtable, runs `CoreObjectDtor` and frees the object when
   `flags & 1`. */

void GameFieldPointDtor(CoreObject *obj, u32 flags)

{
  if (obj != (CoreObject *)0x0) {
    obj->vtable = g_gameFieldPointVtbl;
    CoreObjectDtor(obj,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

