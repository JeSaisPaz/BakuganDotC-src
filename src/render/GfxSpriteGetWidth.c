// bdc 0x089f4914 GfxSpriteGetWidth
#include "bdc.h"

/* Returns a sprite's base width (`+0x70`, float), the size the quad is scaled from by
   GfxSpriteSetScaleRotation and the cell width used by GfxSpriteSetCell. */
float GfxSpriteGetWidth(GfxSprite *sprite)
{
    return sprite->width;
}
