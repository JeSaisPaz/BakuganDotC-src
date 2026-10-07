// bdc 0x089304e0 UiBakuganSelectResetCursor
#include "bdc.h"

/* Resets the selection cursor of `UiBakuganSelect`: restarts the highlight
   pulse (`UiCursorGlowReset`, `UiPulseReset` on the record over `tweens[0x19]`), clears
   `focusZoom`, makes cursor sprite 0x19 visible (flags bit 0, layer mask 2), centred, at scale 1,
   full alpha and add-colour 0.3 grey over the focused grid cell (sprite 0x1a + `cursor`) at depth
   `spriteZ[0x19]`, starts the pulse (`UiPulseInit` with ghost sprite 0x84 and the record over
   `tweens[0x84]`), and restores scale 1 and the layout depth/position of every grid sprite:
   0x1a–0x55 get their `spriteZ` depth, 0x5e–0x71 also `spritePos[cell] + layerOffset[0]`, and the
   0x72–0x7d pairs `spritePos[0x1a + j/2] + layerOffset[1 + j%2]`. Finally re-shows the
   current-Bakugan marker. */

void UiBakuganSelectResetCursor(UiBakuganSelect *self)
{
  GfxSprite **sprites;
  int i;
  int j;

  UiCursorGlowReset();
  self->focusZoom = 0.0f;
  UiPulseReset((UiPulse *)&self->tweens[0x19]);
  sprites = (GfxSprite **)self->base.data;
  sprites[0x19]->flags |= 1;
  sprites[0x19]->layerMask = 2;
  GfxSpriteCenterPivot(sprites[0x19]);
  UiSpriteSetScaleRotation(sprites[0x19], 1.0f, 1.0f, 0.0f);
  sprites[0x19]->alpha = 1.0f;
  sprites[0x19]->addColor[3] = 1.0f;
  sprites[0x19]->addColor[0] = 0.3f;
  sprites[0x19]->addColor[1] = 0.3f;
  sprites[0x19]->addColor[2] = 0.3f;
  sprites[0x19]->posX = sprites[0x1a + self->cursor]->posX;
  sprites[0x19]->posY = sprites[0x1a + self->cursor]->posY;
  sprites[0x19]->posZ = self->spriteZ[0x19];
  UiPulseInit(sprites[0x19], sprites[0x84], (UiPulse *)&self->tweens[0x84]);

  for (i = 0x1a; i < 0x2e; i++) {
    UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
    sprites[i]->posZ = self->spriteZ[i];
  }
  for (i = 0x2e; i < 0x42; i++) {
    UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
    sprites[i]->posZ = self->spriteZ[i];
  }
  for (i = 0x42; i < 0x56; i++) {
    UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
    sprites[i]->posZ = self->spriteZ[i];
  }
  for (i = 0x5e; i < 0x72; i++) {
    UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
    sprites[i]->posZ = self->spriteZ[i];
    sprites[i]->posX = self->spritePos[i - 0x5e + 0x1a][0] + self->layerOffset[0][0];
    sprites[i]->posY = self->spritePos[i - 0x5e + 0x1a][1] + self->layerOffset[0][1];
  }
  for (i = 0x72, j = 0; i < 0x7e; i++, j++) {
    UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
    sprites[i]->posZ = self->spriteZ[i];
    sprites[i]->posX = self->spritePos[0x1a + j / 2][0] + self->layerOffset[1 + j % 2][0];
    sprites[i]->posY = self->spritePos[0x1a + j / 2][1] + self->layerOffset[1 + j % 2][1];
  }
  UiBakuganSelectShowCurrentMark(self, true);
}
