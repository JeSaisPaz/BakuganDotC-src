// bdc 0x0892ecc4 UiBakuganSelectRefreshStatA
#include "bdc.h"

/* Sets the cells of the stat sprites 0x56 (value `u16 +0x1bac` + 5 of the current entry `+0x75`)
   and 0x58.. of the Bakugan select screen (`UiBakuganSelectCtor`, task 371; cursor `+0x74`,
   current entry `+0x75`, owned list `+0x1ba4` with 0xc-byte entries). */

void UiBakuganSelectRefreshStatA(UiBakuganSelect *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  int i;

  for (i = 0x56; i < 0x57; i++) {
    GfxSpriteSetCell(sprites[i], 0.0f,
                     (float)((u16)self->entries[self->current].display[2] + 5));
  }
  for (i = 0x58; i < 0x59; i++) {
    GfxSpriteSetCell(sprites[i], 0.0f,
                     (float)((u16)self->entries[self->current].display[2] + 5));
  }
}
