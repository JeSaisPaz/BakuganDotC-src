// bdc 0x0892e27c UiBakuganSelectUpdateGauges
#include "bdc.h"

/* While `gaugeOn` is set, steps the gauge animation of the Bakugan select screen
   (`UiBakuganSelectCtor`): `gaugeT` += 1/16 and each of the four current gauge values becomes
   `start + t^2 * delta`; once `t` reaches 1 the current values snap to the targets and `gaugeOn`
   is cleared. Then sprites 0xc..0xf get UV rect and size `(value * 63 / 100 + 9) x 16`. */

void UiBakuganSelectUpdateGauges(UiBakuganSelect *self)
{
  float rect[4];
  float t;
  int i;

  if (self->gaugeOn != 0) {
    t = self->gaugeT + 0.0625f;
    self->gaugeT = t;
    for (i = 0; i < 4; i++) {
      self->gaugeVals[i + 4] =
          (u8)(int)((float)self->gaugeVals[i] + t * t * ((float *)self->gaugeDeltaRaw)[i]);
    }
    if (!(t < 1.0f)) {
      for (i = 0; i < 4; i++) {
        self->gaugeVals[i + 4] = self->gaugeVals[i + 8];
      }
      self->gaugeOn = 0;
    }
    for (i = 0; i < 4; i++) {
      rect[0] = 0.0f;
      rect[1] = 0.0f;
      rect[3] = 16.0f;
      rect[2] = (float)((int)self->gaugeVals[i + 4] * 63 / 100 + 9);
      GfxSpriteSetUvRectXYWH(((GfxSprite **)self->base.data)[0xc + i], rect);
      UiSpriteSetSize((float)((int)self->gaugeVals[i + 4] * 63 / 100 + 9), 16.0f,
                      ((GfxSprite **)self->base.data)[0xc + i]);
    }
  }
}
