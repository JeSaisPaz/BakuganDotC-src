// bdc 0x088de8d0 ActorDraw
#include "bdc.h"

/* Draw method of the base actor (vtable `0x08af37e4` slot 8): forwards to `GfxModelDlWriteState`.
   Also called by an NPC draw override. */

void ActorDraw(Actor *self, u32 **dl)

{
  GfxModelDlWriteState(&self->base,dl);
  return;
}

