// bdc 0x08916028 UiUpgradeTweenSlotGrid
#include "bdc.h"

/* Starts the open/close tweens of the upgrade grid of the Bakugan upgrade screen
   (`UiUpgradeCtor`, task 490). When showing (`hide == 0`) it first fills the grid for the
   selected Bakugan `bakugan`: for each of its 6 upgrade slots, an owned slot
   (`upgradeOwned[bakugan][slot]` of the save profile) shows its sprite `0x42 + slot`, an unowned
   one gets its cost (`g_upgradeInfo`) drawn at the position of sprite `0x4c + 5*slot`
   (`UiUpgradeSetCostDigits`); then it sets the slot icon cells (sprites `0x3c + slot`,
   `iconCell`) and the upgrade-id cells (sprites `0x36 + slot`, cell `((id-1)/10, (id-1)%10)`).
   Then sprites 0x2b..0x65 (except 0x32 and 0x35) start an alpha/scale tween (sprites below 0x42
   are made visible first when showing), and the six grid columns (sprites 0x2c/0x36/0x3c/0x42 +
   column and the five 0x48 + 5*column + row sprites) start a Y slide between their own row and
   the first column's row; showing also sets their Z (-50 - column for the first, -100 - column
   for the rest). Finally clears `unk16b0`. */

void UiUpgradeTweenSlotGrid(UiUpgrade *self, u8 hide)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  float z;
  int bakugan;
  int slot;
  int col;
  int row;
  int i;
  u8 id;

  if (hide == 0) {
    for (slot = 0; slot < 6; slot++) {
      SaveProfile *profile = SaveGetProfile();
      sprites = (GfxSprite **)self->base.data;
      if (profile->data->upgradeOwned[self->bakugan][slot] != 0) {
        sprites[0x42 + slot]->flags |= 1;
      } else {
        id = UiUpgradeGetUpgradeId(self->bakugan, slot);
        sprite = sprites[0x4c + slot * 5];
        UiUpgradeSetCostDigits(sprite->posX, sprite->posY, self, g_upgradeInfo[id].cost,
                               0x4c + slot * 5);
      }
    }
    for (slot = 0; slot < 6; slot++) {
      sprite = ((GfxSprite **)self->base.data)[0x3c + slot];
      id = UiUpgradeGetUpgradeId(self->bakugan, slot);
      GfxSpriteSetCell(sprite, 0.0f, (float)g_upgradeInfo[id].iconCell);
    }
    for (slot = 0; slot < 6; slot++) {
      sprite = ((GfxSprite **)self->base.data)[0x36 + slot];
      bakugan = self->bakugan;
      col = (UiUpgradeGetUpgradeId(bakugan, slot) - 1) / 10;
      GfxSpriteSetCell(sprite, (float)col,
                       (float)((UiUpgradeGetUpgradeId(bakugan, slot) - 1) % 10));
    }
    for (i = 0x2b; i < 0x66; i++) {
      if (i == 0x32 || i == 0x35) {
        continue;
      }
      if (i < 0x42) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
      }
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (col = 0; col < 6; col++) {
      ((GfxSprite **)self->base.data)[0x2c + col]->posZ = (float)(-50 - col);
      UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[0x2c + col] - self->spriteY[0x2c], hide,
                        ((GfxSprite **)self->base.data)[0x2c + col], &self->tweens[0x2c + col],
                        0xb);
      z = (float)(-100 - col);
      ((GfxSprite **)self->base.data)[0x36 + col]->posZ = z;
      UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[0x36 + col] - self->spriteY[0x36], hide,
                        ((GfxSprite **)self->base.data)[0x36 + col], &self->tweens[0x36 + col],
                        0xb);
      ((GfxSprite **)self->base.data)[0x3c + col]->posZ = z;
      UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[0x3c + col] - self->spriteY[0x3c], hide,
                        ((GfxSprite **)self->base.data)[0x3c + col], &self->tweens[0x3c + col],
                        0xb);
      ((GfxSprite **)self->base.data)[0x42 + col]->posZ = z;
      UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[0x42 + col] - self->spriteY[0x42], hide,
                        ((GfxSprite **)self->base.data)[0x42 + col], &self->tweens[0x42 + col],
                        0xb);
      for (row = 0; row < 5; row++) {
        i = 0x48 + col * 5 + row;
        ((GfxSprite **)self->base.data)[i]->posZ = z;
        UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[i] - self->spriteY[0x48 + row], hide,
                          ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
      }
    }
  } else {
    for (i = 0x2b; i < 0x66; i++) {
      if (i == 0x32 || i == 0x35) {
        continue;
      }
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (col = 0; col < 6; col++) {
      UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[0x2c] - self->spriteY[0x2c + col], hide,
                        ((GfxSprite **)self->base.data)[0x2c + col], &self->tweens[0x2c + col],
                        0xb);
      UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[0x36] - self->spriteY[0x36 + col], hide,
                        ((GfxSprite **)self->base.data)[0x36 + col], &self->tweens[0x36 + col],
                        0xb);
      UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[0x3c] - self->spriteY[0x3c + col], hide,
                        ((GfxSprite **)self->base.data)[0x3c + col], &self->tweens[0x3c + col],
                        0xb);
      UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[0x42] - self->spriteY[0x42 + col], hide,
                        ((GfxSprite **)self->base.data)[0x42 + col], &self->tweens[0x42 + col],
                        0xb);
      for (row = 0; row < 5; row++) {
        i = 0x48 + col * 5 + row;
        UiTweenBeginSlide(1.0f, 0.0f, self->spriteY[0x48 + row] - self->spriteY[i], hide,
                          ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
      }
    }
  }
  self->unk16b0 = 0;
}
