// bdc 0x08a32568 IoDataOwnerNodeDtor
#include "bdc.h"

/* Destructor (vtable `0x08af7058` entry 1) of the 0x28-byte owner-reference node that
   `IoDataAddOwner` appends to a data request's owner list: reinstalls the vtable, runs
   `CoreNodeDtor` and frees the node when `flags & 1`. */

void IoDataOwnerNodeDtor(CoreNode *node, u32 flags)

{
  if (node != (CoreNode *)0x0) {
    node->vtable = g_ioDataOwnerNodeVtbl;
    CoreNodeDtor(node,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(node,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

