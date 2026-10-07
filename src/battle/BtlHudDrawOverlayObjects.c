// bdc 0x0883a4cc BtlHudDrawOverlayObjects
#include "bdc.h"

/* Draw step of `BtlHudDrawLayers`: while UI window 0 is active (`UiGetWindowActive`) and the
   battle is not over (`g_btlBattleOver` clear), draws the HUD's two overlay sprite layers
   (`GfxSpriteLayerDraw`) into new render packets (`GfxNewRenderPacket`): `overlayObj[0]` at
   depth 107, `overlayObj[1]` at `overlayDepth`; a NULL layer is skipped. */

void BtlHudDrawOverlayObjects(BtlHud *self)
{
    GfxSpriteLayer *layer;

    if (UiGetWindowActive(0) != 0 && g_btlBattleOver == 0) {
        layer = self->overlayObj[0];
        if (layer != NULL) {
            GfxSpriteLayerDraw(layer, GfxNewRenderPacket(107.0f));
        }
        layer = self->overlayObj[1];
        if (layer != NULL) {
            GfxSpriteLayerDraw(layer, GfxNewRenderPacket(self->overlayDepth));
        }
    }
}
