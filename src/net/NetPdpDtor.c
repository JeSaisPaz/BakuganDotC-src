// bdc 0x089d2eec NetPdpDtor
#include "bdc.h"

/* Destructor of the `CONetPDP` object: frees the packet buffer `+0xc` and the object itself when
   `flags & 1`. */

void NetPdpDtor(NetPdp *self, u32 flags)

{
  u8 *ptr;
  
  if (self != (NetPdp *)0x0) {
    ptr = self->buffer;
    if (ptr != (u8 *)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
      self->buffer = (u8 *)0x0;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

