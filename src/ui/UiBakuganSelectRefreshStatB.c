// bdc 0x0892eda8 UiBakuganSelectRefreshStatB
#include "bdc.h"

/* Sets the cells of the stat sprites 0x57 (value `u16 +0x1bae` of the current entry) and 0x59 of
   the Bakugan select screen (`UiBakuganSelectCtor`, task 371; cursor `+0x74`, current entry
   `+0x75`, owned list `+0x1ba4` with 0xc-byte entries). */

void UiBakuganSelectRefreshStatB(UiBakuganSelect *self)
{
  int i;

  for (i = 0x57; i < 0x58; i++) {
    GfxSpriteSetCell(((GfxSprite **)(self->base).data)[i], 0.0f,
                     (float)(u16)self->entries[self->current].display[3]);
  }
  for (i = 0x59; i < 0x5a; i++) {
    GfxSpriteSetCell(((GfxSprite **)(self->base).data)[i], 0.0f,
                     (float)(u16)self->entries[self->current].display[3]);
  }
}
