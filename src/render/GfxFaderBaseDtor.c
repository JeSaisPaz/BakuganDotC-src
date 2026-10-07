// bdc 0x089edbdc GfxFaderBaseDtor
#include "bdc.h"

/* Destructor of the base fader (vtable `0x08af574c`): frees it when `flags & 1`. */

void GfxFaderBaseDtor(GfxFader *self, u32 flags)

{
  if ((self != (GfxFader *)0x0) && (self->vtbl = g_gfxFaderBaseVtbl, (flags & 1) != 0)) {
    MemLock();
    MemFree(self,(char *)0x0,0);
    MemUnlock();
  }
  return;
}

