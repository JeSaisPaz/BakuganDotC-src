// bdc 0x08889f90 UiHpGaugeDtor
#include "bdc.h"

/* Deleting destructor of the HUD hit-point gauge (`UiHpGaugeInit`, 0xa0 bytes): restores the
   gauge vtable `0x08af2194`, runs `CoreNodeDtor` and frees the object when bit 0 of `flags` is
   set. */

void UiHpGaugeDtor(UiHpGauge *self, u32 flags)

{
  if (self != NULL) {
    (self->base).vtable = g_uiHpGaugeVtbl;
    CoreNodeDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,NULL,0);
      MemUnlock();
    }
  }
  return;
}

