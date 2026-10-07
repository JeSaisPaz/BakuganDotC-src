// bdc 0x0897a21c UiCollectionSphereStartBgTween
#include "bdc.h"

/* Starts the fade tween (`UiTweenBegin`, alpha | scale, tweens[0x25..0x29]) of the background
   sprites 0x25..0x29 of `UiCollectionSphere`; `out` = 0 fades them in:
   first sets the button icons of sprites 0x26 (icon 2) and 0x27 (icon 1) (`UiSetButtonIcon`),
   makes every sprite visible and puts it on draw layer 8. `out` != 0 only starts the fade out. */

void UiCollectionSphereStartBgTween(UiCollectionSphere *self, u8 out)
{
  s32 i;

  if (!out) {
    for (i = 0x25; i < 0x2a; i++) {
      if (i == 0x26) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
      } else if (i == 0x27) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
      }
      ((GfxSprite **)self->base.data)[i]->flags |= 1; /* visible */
      ((GfxSprite **)self->base.data)[i]->layerMask = 8;
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0x25; i < 0x2a; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
