// bdc 0x0897e2b4 UiCollectionSphereShowBgForMotionView
#include "bdc.h"

/* Swaps the sprite set of `UiCollectionSphere` for the motion view.
   With `hide` = 0 shows the background sprites 0x26..0x29 (0x26 gets button icon 2, 0x27 icon 1
   via `UiSetButtonIcon`; layer mask 8) and hides the motion-view sprites 0x43/0x44; otherwise
   shows 0x43/0x44 (0x43 gets button icon 1; layer mask 8, alpha 1) and hides 0x26..0x29. */

void UiCollectionSphereShowBgForMotionView(UiCollectionSphere *self, u8 hide)
{
  GfxSprite *sprite;
  int i;

  if (hide == 0) {
    for (i = 0x26; i < 0x2a; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i == 0x26) {
        UiSetButtonIcon(sprite, 2);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      else if (i == 0x27) {
        UiSetButtonIcon(sprite, 1);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 8;
    }
    for (i = 0x43; i < 0x45; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
  }
  else {
    for (i = 0x43; i < 0x45; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i == 0x43) {
        UiSetButtonIcon(sprite, 1);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 8;
      ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
    }
    for (i = 0x26; i < 0x2a; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
  }
}
