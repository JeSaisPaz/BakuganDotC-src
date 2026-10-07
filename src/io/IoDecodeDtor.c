// bdc 0x089fdd70 IoDecodeDtor
#include "bdc.h"

/* Destructor of a decode job (`CODecode`, 0x10b0 bytes) (vtable `0x08af58fc` slot 1): destroys its
   lock, runs `CoreNodeDtor` and frees the node when `flags & 1`. */

void IoDecodeDtor(IoDecodeJob *self, u32 flags)

{
  CoreLock *lock;
  
  if (self != (IoDecodeJob *)0x0) {
    lock = self->lock;
    (self->base).vtable = g_ioDecodeJobVtbl;
    CoreLockDestroy(lock,2);
    CoreNodeDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

