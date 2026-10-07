// bdc 0x08929cec UiHologramViewUpdateFade
#include "bdc.h"

/* Steps the screen fade of the hologram detail view (`UiHologramViewCtor`, task 392):
   `screenFade[2]` (t) += 1/16; fading in (`out` == 0) zooms the sprite layer to
   `screenFade[0] + (1 - (t-1)^2)` with alpha `1 - (t-1)^2`, fading out to `screenFade[0] - t^2`
   with alpha `1 - t^2` (`GfxSpriteLayerSetZoom`). Once t reaches 1 the zoom is set to 1 (in) or
   0 (out) and 1 is returned, else 0. Every frame the alpha `screenFade[3]` is copied to those of
   the 26 screen sprites that have flag bit 0 set. */

u8 UiHologramViewUpdateFade(UiHologramView *self, char out)
{
    float base = self->screenFade[0];
    float t = self->screenFade[2] + 0.0625f;
    GfxSpriteLayer *layer = self->base.spriteLayer;
    int done = 0;
    int i;

    if ((u8)out == 0) {
        float zoom;
        self->screenFade[2] = t;
        zoom = base + (1.0f - (t - 1.0f) * (t - 1.0f));
        self->screenFade[3] = 1.0f - (t - 1.0f) * (t - 1.0f);
        self->screenFade[1] = zoom;
        GfxSpriteLayerSetZoom(zoom, 0.0f, layer, NULL);
        if (!(self->screenFade[2] < 1.0f)) {
            GfxSpriteLayerSetZoom(1.0f, 0.0f, self->base.spriteLayer, NULL);
            self->screenFade[3] = 1.0f;
            done = 1;
        }
    } else {
        float zoom;
        self->screenFade[2] = t;
        zoom = base - t * t;
        self->screenFade[1] = zoom;
        self->screenFade[3] = 1.0f - t * t;
        GfxSpriteLayerSetZoom(zoom, 0.0f, layer, NULL);
        if (!(self->screenFade[2] < 1.0f)) {
            GfxSpriteLayerSetZoom(0.0f, 0.0f, self->base.spriteLayer, NULL);
            done = 1;
        }
    }
    for (i = 0; i < 26; i++) {
        GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
        if ((u8)(sprite->flags & 1) != 0) {
            sprite->alpha = self->screenFade[3];
        }
    }
    return done != 0;
}
