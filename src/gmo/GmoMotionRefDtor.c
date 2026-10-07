// bdc 0x08a32318 GmoMotionRefDtor
#include "bdc.h"

/* Virtual destructor of `GmoMotionRef` (vtable `0x08af5424` entry 1): switches to the base vtable
   `0x08af6fe8`, runs `CoreNodeDtor` and frees the object when bit 0 of `flags` is set. The pack
   data it points to is not owned. */

void GmoMotionRefDtor(GmoMotionRef *ref, u32 flags)

{
  if (ref != (GmoMotionRef *)0x0) {
    ((CoreNode *)ref)->vtable = g_gmoMotionBaseVtbl;
    CoreNodeDtor((CoreNode *)ref,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(ref,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

