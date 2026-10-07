// bdc 0x08982f64 UiCollectionCardStartBgTween
#include "bdc.h"

/* Starts the appear (`out` == 0) or disappear tween (`UiTweenBegin`, start scale 1.0, flags 3)
   of sprites 0x1f..0x23 of `UiCollectionCard` into `tweens[0x1f..0x23]`.
   When appearing, it first points sprite 0x20 at button icon 2 and sprite 0x21 at button icon 1
   (`UiSetButtonIcon`), and sets flag bit 0 and `layerMask` 0x10 on every one of these sprites. */

void UiCollectionCardStartBgTween(UiCollectionCard *self, u8 out)

{
  int i;

  if (out == 0) {
    for (i = 0x1f; i < 0x24; i++) {
      /* the sprite table is re-read after every store, as in the original */
      if (i == 0x20) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
      }
      else if (i == 0x21) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
      }
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 0x10;
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
  else {
    for (i = 0x1f; i < 0x24; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
