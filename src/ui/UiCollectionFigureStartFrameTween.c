// bdc 0x0898cbd4 UiCollectionFigureStartFrameTween
#include "bdc.h"

/* Starts the zoom tween (`UiTweenBegin`, start scale 1.0, flags 3) of the frame/page-label
   sprites 0x1f..0x22 (data `+0x7c..`, tween slots 31-34 at `+0x54c`) of the figure collection
   screen (task 314, `maybe_UiScreen314Ctor`). `out` = 0 opening: sprite 0x1f is made visible,
   the page label is refreshed with `UiCollectionFigureSetPageNumber` (which sets the
   visibility of digit sprites 0x21/0x22), then 0x21/0x22 are tweened and sprite 0x20 is made
   visible and tweened. `out` != 0 closing: the same tweens in the same order, with no visibility
   changes and no page refresh. */

void UiCollectionFigureStartFrameTween(UiCollectionFigure *self, u8 out)
{
    int i;

    if (out == 0) {
        for (i = 0x1f; i < 0x20; i++) {
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
        UiCollectionFigureSetPageNumber(self);
        for (i = 0x21; i < 0x23; i++)
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        for (i = 0x20; i < 0x21; i++) {
            ((GfxSprite **)self->base.data)[i]->flags |= 1;
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        }
    } else {
        for (i = 0x1f; i < 0x20; i++)
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        for (i = 0x21; i < 0x23; i++)
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
        for (i = 0x20; i < 0x21; i++)
            UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
}
