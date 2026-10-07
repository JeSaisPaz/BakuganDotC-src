// bdc 0x0894c6b4 UiBattleRecordResetMenuHighlight
#include "bdc.h"

/* Resets the menu highlight of `UiBattleRecord`: scales sprite 0x12 to 1.15
   with alpha 1, sets sprite 5's colour to (0, 0, 0, 1) and clears the animation counter `animFrame`. */

void UiBattleRecordResetMenuHighlight(UiBattleRecord *self)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  GfxSprite *sprite;

  GfxSpriteSetScaleRotation(sprites[18], 1.15f, 1.15f, 0.0f, false);
  sprite = ((GfxSprite **)self->base.data)[5];
  sprite->addColor[0] = 0.0f;
  sprite->addColor[1] = 0.0f;
  sprite->addColor[2] = 0.0f;
  sprite->addColor[3] = 1.0f;
  ((GfxSprite **)self->base.data)[18]->alpha = 1.0f;
  self->animFrame = 0;
}
