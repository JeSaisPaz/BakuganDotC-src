// bdc 0x08975270 UiCollectionMenuStartFrameTween
#include "bdc.h"

/* Starts the fade tween (`UiTweenBegin`, flags 3, records `tweens[0x14..0x18]`) of the frame
   sprites 0x14..0x18 of `UiCollectionMenu`. When opening (`out == 0`) it
   first points sprite 0x14 at button icon 2 and sprite 0x15 at icon 1 (`UiSetButtonIcon`) and
   sets flag bit 0 on every frame sprite. */

void UiCollectionMenuStartFrameTween(UiCollectionMenu *self, u8 out)
{
  int i;

  if (out == 0) {
    for (i = 0x14; i < 0x19; i++) {
      if (i == 0x14) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
      } else if (i == 0x15) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
      }
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0x14; i < 0x19; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
