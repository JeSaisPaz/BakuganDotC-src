// bdc 0x089a97ac UiTitlePlateStep
#include "bdc.h"

/* Steps the flip animation of the shared title plate set by `UiTitlePlateInit`: fades its alpha
   in by 0.1 per frame (0.2 at 30 fps) while squashing `scaleY` to 0.1, flipping the sprite
   (`GfxSpriteFlipV`) and growing it back, twice (stage `g_uiTitlePlate.step` 0..3). When
   `closing`, shrinks `scaleY` and fades alpha out by 0.125 per frame instead. Returns 1 when the
   animation has finished (stage 4 reached, or alpha 0 when closing), else 0. */

u8 UiTitlePlateStep(u8 closing)
{
    u8 done = 0;
    float delta;

    if (g_gfxDisplay->frameSkip != 0) {
        delta = 0.2f;
    } else {
        delta = 0.1f;
    }

    if (closing) {
        g_uiTitlePlate.sprite->scaleY = g_uiTitlePlate.sprite->scaleY - 0.125f;
        GfxSpriteSetScaleRotation(g_uiTitlePlate.sprite, g_uiTitlePlate.sprite->scaleX,
                                  g_uiTitlePlate.sprite->scaleY, g_uiTitlePlate.sprite->angle,
                                  false);
        g_uiTitlePlate.sprite->alpha = g_uiTitlePlate.sprite->alpha - 0.125f;
        if (g_uiTitlePlate.sprite->alpha <= 0.0f) {
            g_uiTitlePlate.sprite->alpha = 0.0f;
            done = 1;
        }
        return done;
    }

    g_uiTitlePlate.sprite->alpha = g_uiTitlePlate.sprite->alpha + delta;
    if (!(g_uiTitlePlate.sprite->alpha <= 1.0f)) {
        g_uiTitlePlate.sprite->alpha = 1.0f;
    }

    switch (g_uiTitlePlate.step) {
    case 0:
    case 2:
        /* squash, then flip at the thinnest point */
        g_uiTitlePlate.sprite->scaleY = g_uiTitlePlate.sprite->scaleY - delta;
        GfxSpriteSetScaleRotation(g_uiTitlePlate.sprite, g_uiTitlePlate.sprite->scaleX,
                                  g_uiTitlePlate.sprite->scaleY, g_uiTitlePlate.sprite->angle,
                                  false);
        if (g_uiTitlePlate.sprite->scaleY <= 0.1f) {
            GfxSpriteFlipV(g_uiTitlePlate.sprite);
            g_uiTitlePlate.step++;
        }
        break;
    case 1:
    case 3:
        /* grow back to full height */
        g_uiTitlePlate.sprite->scaleY = g_uiTitlePlate.sprite->scaleY + delta;
        GfxSpriteSetScaleRotation(g_uiTitlePlate.sprite, g_uiTitlePlate.sprite->scaleX,
                                  g_uiTitlePlate.sprite->scaleY, g_uiTitlePlate.sprite->angle,
                                  false);
        if (!(g_uiTitlePlate.sprite->scaleY < 1.0f)) {
            if (g_uiTitlePlate.step == 3) {
                done = 1;
            }
            g_uiTitlePlate.step++;
        }
        break;
    default:
        done = 1;
        break;
    }
    return done;
}
