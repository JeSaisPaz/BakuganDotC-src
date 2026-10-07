// bdc 0x08a321a8 GmoMotionBaseDtor
#include "bdc.h"

/* Destructor (vtable `0x08af6fe8` entry 1) of the common base class of the motion registry entries
   `GmoMotionEntry` and `GmoMotionRef`: reinstalls the base vtable, runs `CoreNodeDtor` and
   frees the node when `flags & 1`. */

void GmoMotionBaseDtor(CoreNode *node, u32 flags)

{
  if (node != (CoreNode *)0x0) {
    node->vtable = g_gmoMotionBaseVtbl;
    CoreNodeDtor(node,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(node,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

