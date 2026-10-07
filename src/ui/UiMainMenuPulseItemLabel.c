// bdc 0x089a895c UiMainMenuPulseItemLabel
#include "bdc.h"

/* Pulses the alpha of `item`'s label sprite (`data+0x28+item*4`) while the item is unlocked: each
   frame `t` grows by 1/30 and alpha runs from the snapshot `startAlpha` up (or down) by `t`; once
   `t` reaches 1 the alpha is pinned to 1 (or 0), `t` restarts and the direction flips. Uses
   `slots[10 + item]`. */

void UiMainMenuPulseItemLabel(UiMainMenu *self, u8 item)
{
  UiTween *tween;
  GfxSprite *sprite;
  float t;
  float level;

  if ((self->unlockedMask & (1 << item)) == 0) {
    return;
  }
  tween = &self->slots[10 + item].tween;
  t = tween->t + 0.033333335f;
  level = tween->startAlpha;
  tween->t = t;
  sprite = ((GfxSprite **)self->base.data)[10 + item];
  if (tween->toggle07 == 0) {
    sprite->alpha = level + t;
    if (tween->t < 1.0f) {
      return;
    }
    ((GfxSprite **)self->base.data)[10 + item]->alpha = 1.0f;
    tween->t = 0.0f;
    tween->toggle07 = 1;
  } else {
    sprite->alpha = level - t;
    if (tween->t < 1.0f) {
      return;
    }
    ((GfxSprite **)self->base.data)[10 + item]->alpha = 0.0f;
    tween->t = 0.0f;
    tween->toggle07 = 0;
  }
  tween->startAlpha = ((GfxSprite **)self->base.data)[10 + item]->alpha;
}
