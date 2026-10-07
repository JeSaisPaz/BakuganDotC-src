// bdc 0x0889d01c BtlStageCreateSkySprite
#include "bdc.h"

/* Creates a sky billboard sprite with texture `name` (`ffx_sun`/`ffx_moon`) on
   `g_billboardSpriteLayer` (`GfxSpriteLayerCreateBillboardByName`): blend mode `blendMode &
   0xffff`, billboard mode 0, flag bit 0 set, tint and alpha copied from `g_colorWhite`, position
   copied from `pos` (four floats), width and height `-size`, depth `size`, `maybe_sizeW` 0, then
   stencil write with ref 0xb2 (`GfxSpriteSetStencilWrite`). Returns the sprite. Used by
   `BtlStageCreateSkyLights`. */
GfxSprite *BtlStageCreateSkySprite(float size, const char *name, float *pos, u32 blendMode)
{
    GfxSprite *sprite;

    sprite = GfxSpriteLayerCreateBillboardByName(g_billboardSpriteLayer, name);
    sprite->blendMode = blendMode & 0xffff;
    sprite->billboardMode = 0;
    sprite->flags |= 1;
    sprite->tint[0] = g_colorWhite.x;
    sprite->tint[1] = g_colorWhite.y;
    sprite->tint[2] = g_colorWhite.z;
    sprite->alpha = g_colorWhite.w;
    sprite->posX = pos[0];
    sprite->posY = pos[1];
    sprite->posZ = pos[2];
    sprite->posW = pos[3];
    sprite->width = -size;
    sprite->height = -size;
    sprite->depth = size;
    sprite->maybe_sizeW = 0.0f;
    GfxSpriteSetStencilWrite(sprite, true, 0xb2);
    return sprite;
}
