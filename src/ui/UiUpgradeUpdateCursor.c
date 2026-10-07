// bdc 0x08916740 UiUpgradeUpdateCursor
#include "bdc.h"

/* Moves the selection highlight of the Bakugan upgrade screen (`UiUpgradeCtor`, task 490; selected
   Bakugan `bakugan`, focused element `focus`) and refreshes the frame colours. Resets the glow
   (`UiCursorGlowReset`) and `unk169c`. When `focus` is 6 (the OK button) the cursor is sprite 53
   (pulse record `tweens[0x35]`), shown over the OK button sprite 51 and sprite 50 is hidden; otherwise
   the cursor is sprite 50 (pulse `tweens[0x32]`), shown 1000 units behind its depth over slot frame
   sprite `44 + focus`, and sprite 53 is hidden. The cursor gets scale 1, alpha 1, add colour
   0.3/0.3/0.3/1, and its pulse ghost (sprite 102, record `tweens[0x66]`) is restarted (`UiPulseInit`).
   The slot frames 44..49 are lit (add colour 0.3, `UiUpgradeSetSlotFrameLit`) for the focused slot,
   unless that slot (> 0) has a prerequisite (`g_upgradePrereqFlags`) whose predecessor is not owned
   (`upgradeOwned[bakugan][focus - 1]`), in which case it is left untouched; the others are unlit
   (add colour 0). The OK button 51 is lit iff `focus` is 6 (`UiUpgradeSetOkButtonLit`). Sprites
   44..49, 51, 52, 60..71 get scale 1 and depth `spriteZ[i]`. */

void UiUpgradeUpdateCursor(UiUpgrade *self)
{
  GfxSprite *sprite;
  int i;

  UiCursorGlowReset();
  self->unk169c = 0.0f;
  if (self->focus == 6) {
    UiPulseReset((UiPulse *)&self->tweens[0x35]);
    sprite = ((GfxSprite **)self->base.data)[53];
    sprite->flags |= 1;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[53], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[53]->alpha = 1.0f;
    sprite = ((GfxSprite **)self->base.data)[53];
    sprite->addColor[0] = 0.3f;
    sprite->addColor[1] = 0.3f;
    sprite->addColor[2] = 0.3f;
    sprite->addColor[3] = 1.0f;
    ((GfxSprite **)self->base.data)[53]->posZ = self->spriteZ[53];
    ((GfxSprite **)self->base.data)[53]->posX = ((GfxSprite **)self->base.data)[51]->posX;
    ((GfxSprite **)self->base.data)[53]->posY = ((GfxSprite **)self->base.data)[51]->posY;
    UiPulseInit(((GfxSprite **)self->base.data)[53], ((GfxSprite **)self->base.data)[102],
                (UiPulse *)&self->tweens[0x66]);
    ((GfxSprite **)self->base.data)[50]->flags &= ~1u;
  } else {
    UiPulseReset((UiPulse *)&self->tweens[0x32]);
    sprite = ((GfxSprite **)self->base.data)[50];
    sprite->flags |= 1;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[50], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[50]->alpha = 1.0f;
    sprite = ((GfxSprite **)self->base.data)[50];
    sprite->addColor[0] = 0.3f;
    sprite->addColor[1] = 0.3f;
    sprite->addColor[2] = 0.3f;
    sprite->addColor[3] = 1.0f;
    ((GfxSprite **)self->base.data)[50]->posZ = self->spriteZ[50] - 1000.0f;
    ((GfxSprite **)self->base.data)[50]->posX =
        ((GfxSprite **)self->base.data)[44 + self->focus]->posX;
    ((GfxSprite **)self->base.data)[50]->posY =
        ((GfxSprite **)self->base.data)[44 + self->focus]->posY;
    UiPulseInit(((GfxSprite **)self->base.data)[50], ((GfxSprite **)self->base.data)[102],
                (UiPulse *)&self->tweens[0x66]);
    ((GfxSprite **)self->base.data)[53]->flags &= ~1u;
  }

  /* slot frames 44..49 (slot = i - 44) */
  for (i = 44; i < 50; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    if (i - 44 == self->focus) {
      if (self->focus > 0 &&
          g_upgradePrereqFlags[g_upgradeClassTable[self->bakugan] * 6 + self->focus] != 0 &&
          SaveGetProfile()->data->upgradeOwned[self->bakugan][self->focus - 1] == 0) {
        continue;
      }
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.3f;
      sprite->addColor[1] = 0.3f;
      sprite->addColor[2] = 0.3f;
      sprite->addColor[3] = 1.0f;
      UiUpgradeSetSlotFrameLit(self, ((GfxSprite **)self->base.data)[i], true);
    } else {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
      UiUpgradeSetSlotFrameLit(self, ((GfxSprite **)self->base.data)[i], false);
    }
  }

  for (i = 60; i < 66; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = 66; i < 72; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }

  /* OK button (sprite 51) */
  for (i = 51; i < 52; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    sprite = ((GfxSprite **)self->base.data)[i];
    if (self->focus == 6) {
      sprite->addColor[0] = 0.3f;
      sprite->addColor[1] = 0.3f;
      sprite->addColor[2] = 0.3f;
      sprite->addColor[3] = 1.0f;
      UiUpgradeSetOkButtonLit(self, ((GfxSprite **)self->base.data)[i], true);
    } else {
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
      UiUpgradeSetOkButtonLit(self, ((GfxSprite **)self->base.data)[i], false);
    }
  }

  for (i = 52; i < 53; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
}
