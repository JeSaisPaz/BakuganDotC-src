// bdc 0x089f491c GfxSpriteGetHeight
#include "bdc.h"

/* Returns a sprite's base height (`+0x74`, float); counterpart of `GfxSpriteGetWidth`. */
float GfxSpriteGetHeight(GfxSprite *sprite)
{
    return sprite->height;
}
