// bdc 0x08971b70 UiOptionUpdateValueSprite
#include "bdc.h"

/* Updates the value sprite of the selected row of `UiOption` to its new value (cell,
   or `UiOptionSetRoundsTexture` for row 3). */

void UiOptionUpdateValueSprite(UiOption *self)

{
  s8 row = (s8)self->cursor;
  GfxSprite **sprites;

  if (row < 2) {
    if (row >= 0) {
      sprites = (GfxSprite **)self->base.data;
      if (row > 0) {
        GfxSpriteSetCell(sprites[0x94 / 4], 0.0f, (float)self->values[1]);
        return;
      }
      GfxSpriteSetCell(sprites[0x90 / 4], 0.0f, (float)self->values[0]);
      return;
    }
  } else {
    if (row < 3) {
      sprites = (GfxSprite **)self->base.data;
      GfxSpriteSetCell(sprites[0x98 / 4], 0.0f, (float)self->values[2]);
      return;
    }
    if (row < 4) {
      sprites = (GfxSprite **)self->base.data;
      UiOptionSetRoundsTexture(self, sprites[0x9c / 4], self->values[3]);
    }
  }
}
