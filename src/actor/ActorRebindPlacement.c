// bdc 0x088e0534 ActorRebindPlacement
#include "bdc.h"

/* Reuses the already spawned actor in slot `slot` of the field's actor table (`g_gameFieldCharSet->actors`):
   attaches the placement record, plays its idle motion and applies the placement. Returns the
   actor. */

void *ActorRebindPlacement(u32 slot, s32 *record)

{
  Actor *self;

  ActorModelIdFromCode(((GameFieldPlacedChar *)record)->modelCode);
  self = (Actor *)g_gameFieldCharSet->actors[slot & 0xff];
  self->placement = record;
  ActorPlayPlacedMotion(self,1);
  ActorApplyPlacement(self);
  return self;
}
