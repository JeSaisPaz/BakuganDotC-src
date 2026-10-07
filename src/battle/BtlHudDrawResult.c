// bdc 0x0883a550 BtlHudDrawResult
#include "bdc.h"

/* Result-phase HUD draw: each existing sprite layer gets a new render packet and is drawn into it
   (GfxSpriteLayerDraw): the result, message, rating and arena layers with sort keys 115, 120,
   125 and 130, then overlayObj[1] with the key `overlayDepth`. */

void BtlHudDrawResult(BtlHud *self)
{
    GfxSpriteLayer *overlay;

    if (self->resultLayer != NULL) {
        GfxSpriteLayerDraw(self->resultLayer, GfxNewRenderPacket(115.0f));
    }
    if (self->msgLayer != NULL) {
        GfxSpriteLayerDraw(self->msgLayer, GfxNewRenderPacket(120.0f));
    }
    if (self->ratingLayer != NULL) {
        GfxSpriteLayerDraw(self->ratingLayer, GfxNewRenderPacket(125.0f));
    }
    if (self->arenaLayer != NULL) {
        GfxSpriteLayerDraw(self->arenaLayer, GfxNewRenderPacket(130.0f));
    }
    overlay = self->overlayObj[1];
    if (overlay != NULL) {
        GfxSpriteLayerDraw(overlay, GfxNewRenderPacket(self->overlayDepth));
    }
}
