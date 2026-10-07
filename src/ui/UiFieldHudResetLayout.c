// bdc 0x088cebb8 UiFieldHudResetLayout
#include "bdc.h"

/* Resets the field HUD (task 3001, `UiFieldHudCtor`; sprite array `base.data`, player `player`):
   converts four panel sprites (3, 0x11, 2, 0x12) to a top-left pivot and rebuilds their matrix,
   puts the 67 layout sprites 0..0x42 back at their positions from layout 0xc (`UiLayoutGetEntry`),
   hides them and sprites 0x43..0x47 at alpha 1, clears the player's prompt flags
   (`promptA`, `idleHint`) and the prompt state, sets `promptY = -48` and clears the hint printer. */

void UiFieldHudResetLayout(UiFieldHud *self)
{
  s16 *entry;
  UiTextPrinter *printer;
  int i;

  GfxSpriteSetTopLeftPivot(((GfxSprite **)self->base.data)[3]);
  GfxSpriteResetMatrix(((GfxSprite **)self->base.data)[3]);
  GfxSpriteSetTopLeftPivot(((GfxSprite **)self->base.data)[0x11]);
  GfxSpriteResetMatrix(((GfxSprite **)self->base.data)[0x11]);
  GfxSpriteSetTopLeftPivot(((GfxSprite **)self->base.data)[2]);
  GfxSpriteResetMatrix(((GfxSprite **)self->base.data)[2]);
  GfxSpriteSetTopLeftPivot(((GfxSprite **)self->base.data)[0x12]);
  GfxSpriteResetMatrix(((GfxSprite **)self->base.data)[0x12]);

  for (i = 0; i < 0x43; i++) {
    entry = UiLayoutGetEntry(0xc, i);
    ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    ((GfxSprite **)self->base.data)[i]->posX = (float)entry[0];
    ((GfxSprite **)self->base.data)[i]->posY = (float)entry[1];
    ((GfxSprite **)self->base.data)[i]->posZ = (float)entry[2];
  }
  for (i = 0x43; i < 0x48; i++) {
    ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
  }

  self->player->promptA = 0;
  self->player->idleHint = 0;
  self->promptTimer = 0;
  self->promptShown = 0;
  self->promptVisible = 0;
  self->promptY = -48.0f;
  printer = self->hintPrinter;
  if (printer != NULL) {
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;
  }
}
