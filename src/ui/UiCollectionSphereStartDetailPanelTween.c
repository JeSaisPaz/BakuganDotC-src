// bdc 0x0897ca38 UiCollectionSphereStartDetailPanelTween
#include "bdc.h"

/* Starts the tween of the detail panel sprites (sprite slots 43-44 and 46-47 of the screen's
   sprite table, tweens 43-44 / 46-47) of the sphere (Bakugan figure) collection screen
   (task 312, `maybe_UiScreen312Ctor`). When an entry is opened (`out` = 0) each sprite is
   made visible (flag bit 0) on layer mask 4 and the first one is reset to 50 % grey, alpha 0;
   then UiTweenBegin (scale 1.0, flags 3) runs for every sprite, fading in or out per `out`. */

void UiCollectionSphereStartDetailPanelTween(UiCollectionSphere *self, u8 out)
{
  int i;

  if (out == 0) {
    for (i = 0x2b; i < 0x2d; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      if (i == 0x2b) {
        GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
        sprite->tint[0] = 0.5f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 0.5f;
        sprite->alpha = 0.0f;
      }
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x2e; i < 0x30; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
  else {
    for (i = 0x2b; i < 0x2d; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x2e; i < 0x30; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
