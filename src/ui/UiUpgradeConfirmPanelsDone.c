// bdc 0x08913ae0 UiUpgradeConfirmPanelsDone
#include "bdc.h"

/* Advances one frame of the appear/disappear tweens (`UiTweenUpdate`, scale 1.0 -> 1.0 over 16
   frames, tween flags 1, `hide` = fade out) of sprites 0xe, 0x10 and 0x28..0x2a of the Bakugan
   upgrade screen (`UiUpgradeCtor`, task 490). Returns true once at least one of these tweens has finished (sum of `UiTweenUpdate` results, kept as u8, is non-zero). */

bool UiUpgradeConfirmPanelsDone(UiUpgrade *self, u8 hide)

{
  GfxSprite **sprites;
  u8 running;
  int i;

  running = 0;
  for (i = 0xe; i < 0xf; i++) {
    sprites = (GfxSprite **)self->base.data;
    running = (u8)(running + UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1));
  }
  for (i = 0x10; i < 0x11; i++) {
    sprites = (GfxSprite **)self->base.data;
    running = (u8)(running + UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1));
  }
  for (i = 0x28; i < 0x2b; i++) {
    sprites = (GfxSprite **)self->base.data;
    running = (u8)(running + UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1));
  }
  return running != 0;
}
