// bdc 0x089cdf08 PadBaseDtor
#include "bdc.h"

/* Base-class destructor of the controller object: restores the base vtable `0x08af532c`, frees the
   buffer at `+0x40` (if any) and, when bit 0 of `flags` is set, frees `pad` itself. */

void PadBaseDtor(PadState *pad, u32 flags)

{
  void *ptr;
  
  if (pad != (PadState *)0x0) {
    ptr = pad->buffer;
    pad->vtable = &g_padBaseVtable;
    if (ptr != (void *)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
      pad->buffer = (void *)0x0;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(pad,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

