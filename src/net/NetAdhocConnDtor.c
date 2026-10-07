// bdc 0x089d3bf4 NetAdhocConnDtor
#include "bdc.h"

/* Destructor of the COPSPNet connection object: destroys its lock (`+0x30`) and frees the object
   when bit 0 of `flags` is set (GCC 2.x deleting-destructor convention). */

void NetAdhocConnDtor(NetAdhocConn *self, u32 flags)

{
  if (self != (NetAdhocConn *)0x0) {
    if (self->lock != (CoreLock *)0x0) {
      CoreLockDestroy(self->lock,3);
      self->lock = (CoreLock *)0x0;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

