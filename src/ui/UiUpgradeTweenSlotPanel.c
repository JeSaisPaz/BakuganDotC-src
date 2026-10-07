// bdc 0x08915714 UiUpgradeTweenSlotPanel
#include "bdc.h"

/* Starts the open (`hide == 0`) or close tweens of the upgrade-slot panel of the Bakugan upgrade
   screen (task 490, `maybe_UiScreen490Ctor`; `"up_grade.fab"`, `"DMUpgrade"` texts). Sprites
   0xd, 0xf and 0x27 get `UiTweenBegin` (alpha+scale, flags 3); sprites 0x11..0x17, 0x18..0x1e
   and 0x1f..0x25 (the three digit counters) get `UiTweenBeginSlide` with no slide (flags 9).
   Each sprite uses the tween of the same index. When opening, the first three sprites are made
   visible (`flags |= 1`), the digit sprites start at alpha 0, and the counters are filled with
   `UiUpgradeSetNumber`: the profile's points (at sprite 0x17's position), profile word 0x2d
   (sprite 0x1e, color 1) and points minus that word (sprite 0x25). */

void UiUpgradeTweenSlotPanel(UiUpgrade *self, u8 hide)
{
  GfxSprite **sprites;
  GfxSprite *anchor;
  s32 points;
  u32 word;
  int i;

  if (hide == 0) {
    for (i = 0xd; i < 0xe; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0xf; i < 0x10; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x27; i < 0x28; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags |= 1;
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }

    points = SaveGetProfile()->data->points;
    anchor = ((GfxSprite **)self->base.data)[0x17];
    UiUpgradeSetNumber(self, points, 0x17, 0, anchor->posX, anchor->posY);
    for (i = 0x11; i < 0x18; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->alpha = 0.0f;
      UiTweenBeginSlide(1.0f, 0.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 9);
    }

    word = SaveProfileGetWord(SaveGetProfile(), 0x2d);
    anchor = ((GfxSprite **)self->base.data)[0x1e];
    UiUpgradeSetNumber(self, (s32)word, 0x1e, 1, anchor->posX, anchor->posY);
    for (i = 0x18; i < 0x1f; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->alpha = 0.0f;
      UiTweenBeginSlide(1.0f, 0.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 9);
    }

    points = SaveGetProfile()->data->points;
    word = SaveProfileGetWord(SaveGetProfile(), 0x2d);
    anchor = ((GfxSprite **)self->base.data)[0x25];
    UiUpgradeSetNumber(self, points - (s32)word, 0x25, 0, anchor->posX, anchor->posY);
    for (i = 0x1f; i < 0x26; i++) {
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->alpha = 0.0f;
      UiTweenBeginSlide(1.0f, 0.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 9);
    }
  }
  else {
    for (i = 0xd; i < 0xe; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0xf; i < 0x10; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x27; i < 0x28; i++) {
      UiTweenBegin(1.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x11; i < 0x18; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 9);
    }
    for (i = 0x18; i < 0x1f; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 9);
    }
    for (i = 0x1f; i < 0x26; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, 0.0f, hide, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 9);
    }
  }
}
