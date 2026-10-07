// bdc 0x0898ce04 UiCollectionFigureFrameDone
#include "bdc.h"

/* Advances by one frame (`UiTweenUpdate` scale 1.0 -> 1.0 over 16 frames, flags 1) the
   tweens of the frame/page-label sprites 0x1f, 0x21, 0x22, 0x20 (in that order; tween slots
   `tweens[0x1f..0x22]` at `+0x54c`) of `UiCollectionFigure` started by
   `UiCollectionFigureStartFrameTween`. Returns true if at least one of the four tweens reports
   finished (the u8 count of finished tweens is non-zero); they all start together with the same
   length, so this means "done". */

bool UiCollectionFigureFrameDone(UiCollectionFigure *self, u8 out)
{
    u8 done = 0;
    int i;

    for (i = 0x1f; i < 0x20; i++)
        done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
    for (i = 0x21; i < 0x23; i++)
        done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
    for (i = 0x20; i < 0x21; i++)
        done += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
    return done != 0;
}
