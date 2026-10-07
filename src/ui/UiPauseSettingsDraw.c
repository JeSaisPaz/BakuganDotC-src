// bdc 0x089abb60 UiPauseSettingsDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiPauseSettings screen (task id 301): draws the screen background
   (`UiScreenDrawBg`); then draws the sprite layer in two render packets at depths 850 (layer
   mask 1) and 1000 (layer mask 2). Does nothing without a sprite layer. */

void UiPauseSettingsDraw(UiPauseSettings *self)
{
    void *packet;

    if (self->base.spriteLayer != NULL) {
        UiScreenDrawBg(&self->base);
        packet = GfxNewRenderPacket(850.0f);
        GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
        GfxSpriteLayerDraw(self->base.spriteLayer, packet);
        packet = GfxNewRenderPacket(1000.0f);
        GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 2);
        GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    }
}
