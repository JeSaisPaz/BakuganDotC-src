// bdc 0x089889fc UiCollectionTheaterStartFrameTween
#include "bdc.h"

/* Starts the zoom tween (`UiTweenBegin`, start scale 1.0, flags 3) of the frame/page-label
   sprites 0x25..0x28 (tween slots 37-40, `frameTweens`) of the theater collection screen
   (task 315, `maybe_UiScreen315Ctor`). `out` = 0 opening: sprite 0x25 is made visible, the
   page label is refreshed with `UiCollectionTheaterSetPageNumber` (which sets the visibility
   of digit sprites 0x27/0x28), then 0x27/0x28 are tweened and sprite 0x26 is made visible and
   tweened. `out` != 0 closing: the same tweens in the same order, with no visibility changes and
   no page refresh. */

void UiCollectionTheaterStartFrameTween(UiScreen *screen, u8 out)
{
    UiCollectionTheater *self = (UiCollectionTheater *)screen;
    int i;

    if (out == 0) {
        for (i = 0x25; i < 0x26; i++) {
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i],
                         &self->frameTweens[i - 0x25], 3);
        }
        UiCollectionTheaterSetPageNumber(screen);
        for (i = 0x27; i < 0x29; i++)
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i],
                         &self->frameTweens[i - 0x25], 3);
        for (i = 0x26; i < 0x27; i++) {
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i],
                         &self->frameTweens[i - 0x25], 3);
        }
    } else {
        for (i = 0x25; i < 0x26; i++)
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i],
                         &self->frameTweens[i - 0x25], 3);
        for (i = 0x27; i < 0x29; i++)
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i],
                         &self->frameTweens[i - 0x25], 3);
        for (i = 0x26; i < 0x27; i++)
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i],
                         &self->frameTweens[i - 0x25], 3);
    }
}
