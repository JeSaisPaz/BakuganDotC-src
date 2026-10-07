// bdc 0x08913c50 UiUpgradeDetailPanelsDone
#include "bdc.h"

/* Advances one frame of the appear/disappear tweens (`UiTweenUpdate`, scale 1.0 -> 1.0 over 16
   frames, `hide` = fade out) of sprites 0x2b..0x65 of the Bakugan upgrade screen (`UiUpgradeCtor`,
   task 490), skipping sprites 0x32 and 0x35; sprites 0x2b, 0x33 and 0x34 use tween flags 1, all
   others flags 9. Returns true once at least one of these tweens has finished (sum of `UiTweenUpdate` results, kept as u8, is non-zero). */

bool UiUpgradeDetailPanelsDone(UiUpgrade *self, u8 hide)

{
  GfxSprite **sprites;
  UiTween *tween;
  u8 running;
  u8 flags;
  int i;

  running = 0;
  tween = &self->tweens[0x2b];
  for (i = 0x2b; i < 0x66; i++, tween++) {
    if (i == 0x32 || i == 0x35) {
      continue;
    }
    if (i == 0x2b || i == 0x32 || i == 0x33 || i == 0x34 || i == 0x35) {
      flags = 1;
    } else {
      flags = 9;
    }
    sprites = (GfxSprite **)self->base.data;
    running = (u8)(running + UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], tween, flags));
  }
  return running != 0;
}
