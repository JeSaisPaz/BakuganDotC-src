// bdc 0x089eddb8 GfxFaderSetEnabled
#include "bdc.h"

/* Stores `enabled` in the fader's first byte and calls vtable slot +0x14 of the overlay sub-object
   (at `+0x50`, adjusted by the vtable's `+0x10` delta) so the overlay rectangle reacts.
   `ScriptOpFade` cmd 4. */

void GfxFaderSetEnabled(GfxFader *self, u8 enabled)

{
  self->active = enabled;
  ((void (*)(void *))self->vtbl[2].fn)((u8 *)self + self->vtbl[2].delta);
  return;
}

