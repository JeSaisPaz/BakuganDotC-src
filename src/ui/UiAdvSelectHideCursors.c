// bdc 0x08919ed8 UiAdvSelectHideCursors
#include "bdc.h"

/* Hides the cursor sprites 0x11 and 0x27 of the adventure partner-select screen
   (`UiAdvSelectCtor`, task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner,
   locked, ?}` slots) (clears bit 0 of `+0xd0`). */

void UiAdvSelectHideCursors(UiAdvSelect *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[0x11]->flags &= ~1u;
  sprites[0x27]->flags &= ~1u;
  return;
}

