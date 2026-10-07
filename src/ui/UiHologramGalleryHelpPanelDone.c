// bdc 0x08921858 UiHologramGalleryHelpPanelDone
#include "bdc.h"

/* Advances the help-panel tweens of `UiHologramGalleryTweenHelpPanel` and returns true once any
   of them has finished (`UiTweenUpdate` returns true when finished). */

bool UiHologramGalleryHelpPanelDone(UiHologramGallery *self, u8 hide)
{
    u8 finished = 0;
    u8 i = 0;
    u8 idx;

    while ((idx = UiHologramGalleryHelpPanelSprite(self, i)) != 0xff) {
        GfxSprite **sprites = (GfxSprite **)self->base.data;
        finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[idx], &self->tweens[idx], 1);
        i++;
    }
    return finished != 0;
}
