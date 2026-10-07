// bdc 0x08988c2c UiCollectionTheaterUpdateFrameTween
#include "bdc.h"

/* Advances by one frame (`UiTweenUpdate` scale 1.0 -> 1.0 over 16 frames, flags 1) the
   frame/page-label tweens `frameTweens` of sprites 0x25, 0x27, 0x28, 0x26 (in that order) of
   `UiCollectionTheater` started by
   `UiCollectionTheaterStartFrameTween`. Returns true if at least one of the four tweens reports
   finished (the u8 count of finished tweens is non-zero); they all start together with the same
   length, so this means "done". */

bool UiCollectionTheaterUpdateFrameTween(UiScreen *screen, u8 out)
{
    UiCollectionTheater *self = (UiCollectionTheater *)screen;
    u8 done = 0;
    int i;

    for (i = 0x25; i < 0x26; i++)
        done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->frameTweens[i - 0x25], 1);
    for (i = 0x27; i < 0x29; i++)
        done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->frameTweens[i - 0x25], 1);
    for (i = 0x26; i < 0x27; i++)
        done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->frameTweens[i - 0x25], 1);
    return done != 0;
}
