// bdc 0x08970ef4 UiOptionRefreshValues
#include "bdc.h"

/* Shows the four value sprites 0x24..0x27 of `UiOption` with the cells of the
   current values `values[0..3]` (row 3 through `UiOptionSetRoundsTexture`); greys out row 0
   under `SaveGetProfileFlag0` and row 3 when its bit in `enabledRows` is clear. */

void UiOptionRefreshValues(UiOption *self)
{
  int i;

  for (i = 0x24; i < 0x28; i++) {
    int row = i - 0x24;

    UiOptionShowSprite(&self->base, ((GfxSprite **)self->base.data)[i]);
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    self->spriteY[i] = ((GfxSprite **)self->base.data)[i]->posY;
    if (row < 2) {
      if (row < 0) {
        continue;
      }
      if (row <= 0) {
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f, (float)self->values[0]);
        if (SaveGetProfileFlag0() != 0) {
          GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];

          sprite->tint[0] = 0.5f;
          sprite->tint[1] = 0.5f;
          sprite->tint[2] = 0.5f;
          sprite->alpha = 1.0f;
        }
      }
      else {
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f, (float)self->values[1]);
      }
    }
    else if (row < 3) {
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f, (float)self->values[2]);
    }
    else if (row < 4) {
      UiOptionSetRoundsTexture(self, ((GfxSprite **)self->base.data)[i], self->values[3]);
      if ((self->enabledRows & (1 << row)) == 0) {
        GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];

          sprite->tint[0] = 0.5f;
          sprite->tint[1] = 0.5f;
          sprite->tint[2] = 0.5f;
          sprite->alpha = 1.0f;
      }
    }
  }
}
