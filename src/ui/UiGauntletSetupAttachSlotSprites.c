// bdc 0x0893462c UiGauntletSetupAttachSlotSprites
#include "bdc.h"

/* Keeps the decorations of the four card slots of `UiGauntletSetup` attached
   to their slot: for slot i (0–3) the Y of sprites 0x22+i, 0x32+i, 0x14+i and 0x18+i is set to
   the Y of slot sprite 0x0e+i plus the per-row offsets `+0x1a58`, `+0x1a5c`, `+0x1a60`, `+0x1a64`
   scaled by the slot sprite's Y scale (`+0x94`). */

void UiGauntletSetupAttachSlotSprites(UiGauntletSetup *self)
{
  int i;
  static const int dst[4] = {34, 50, 20, 24};
  int k;

  for (i = 0; i < 4; i++) {
    for (k = 0; k < 4; k++) {
      GfxSprite **spr = (GfxSprite **)self->base.data;
      GfxSprite *slot = spr[14 + i];
      spr[dst[k] + i]->posY = slot->posY + self->slotOffsets[12 + k] * slot->scaleY;
    }
  }
}
