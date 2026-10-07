// bdc 0x0898600c UiCollectionCardResetCursor
#include "bdc.h"

/* Resets the cursor of `UiCollectionCard`: resets the cursor glow and
   `pulse`; only while `targetStep` is 0 it also puts the cursor sprite 4 (shown, centred pivot,
   unit scale, full alpha, add colour 0.3) on the selected cell with that cell's depth, restarts
   its pulse with the ghost sprite 65, resets scale and depth of cells 0-3 (selected cell lit with
   frame "waku_4_a", the others unlit with "waku_4_b") and of sprites 23-26, and moves the card
   sprites 13-16 of non-empty slots to `bobPos[i][1] - (1 - cos(bobT * pi)) * 4`. */

void UiCollectionCardResetCursor(UiCollectionCard *self)

{
  GfxSprite **sprites;
  GfxSprite *cell;
  s32 i;

  UiCursorGlowReset();
  self->pulse = 0.0f;
  if ((s8)self->targetStep != 0) {
    return;
  }
  UiPulseReset((UiPulse *)&self->tweens[4]);
  sprites = (GfxSprite **)self->base.data;
  sprites[4]->flags |= 1;
  GfxSpriteCenterPivot(sprites[4]);
  UiSpriteSetScaleRotation(sprites[4], 1.0f, 1.0f, 0.0f);
  sprites[4]->alpha = 1.0f;
  sprites[4]->addColor[3] = 1.0f;
  sprites[4]->addColor[0] = 0.3f;
  sprites[4]->addColor[1] = 0.3f;
  sprites[4]->addColor[2] = 0.3f;
  sprites[4]->posZ = self->spriteZ[4];
  sprites[4]->posX = sprites[self->cursor]->posX;
  sprites[4]->posY = sprites[self->cursor]->posY;
  UiPulseInit(sprites[4], sprites[65], (UiPulse *)&self->tweens[65]);

  for (i = 0; i < 4; i++) {
    UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
    sprites[i]->posZ = self->spriteZ[i];
    cell = sprites[i];
    if (i == self->cursor) {
      cell->addColor[0] = 0.3f;
      cell->addColor[1] = 0.3f;
      cell->addColor[2] = 0.3f;
      cell->addColor[3] = 1.0f;
      UiCollectionCardSetCellFrame(self, sprites[i], 1);
    } else {
      cell->addColor[0] = 0.0f;
      cell->addColor[1] = 0.0f;
      cell->addColor[2] = 0.0f;
      cell->addColor[3] = 1.0f;
      UiCollectionCardSetCellFrame(self, sprites[i], 0);
    }
  }

  for (i = 23; i < 27; i++) {
    UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
    sprites[i]->posZ = self->spriteZ[i];
  }

  for (i = 0; i < 4; i++) {
    if (self->slots[i + self->page * 4] != 0xff) {
      float y = self->bobPos[i][1];
      float c = __builtin_cosf(self->bobT * 3.14159274f);

      sprites[13 + i]->posY = y - (1.0f - c) * 0.5f * 8.0f;
    }
  }
  return;
}
