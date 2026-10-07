// bdc 0x088237c4 GfxEffectUpdateNow
#include "bdc.h"

/* Forces an immediate update of `effect` `effect`: resets its last-update tick
   `+0x208` to -1 (so `GfxEffectUpdate` does not skip it) and calls its virtual update (vtable
   `+0x14`, slot 2). Used right after spawning attachments (`BtlBakuganCreateAttachments`,
   `BtlBakuganState03Update`, …). */

void GfxEffectUpdateNow(GfxEffect *effect)

{
  const VtblEntry *update = &((const VtblEntry *)effect->base.vtable)[2];

  effect->lastTick = -1;
  ((void (*)(void *))update->fn)((u8 *)effect + update->delta);
  return;
}

