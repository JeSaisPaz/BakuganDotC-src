// bdc 0x088d3d88 GameStageCreateBillboard
#include "bdc.h"

/* Creates a billboard sprite with texture `texture` on the 3D sprite layer
   `g_billboardSpriteLayer` (`GfxSpriteLayerCreateBillboardByName`) at `pos` (four floats copied), white, sized
   ±`size` around its centre, with `id` as its blend mode (`+0xdc`), billboard mode 0, flag bit 0 set, and stencil
   write of ref `0xb2` (`GfxSpriteSetStencilWrite`). Returns the sprite. */

void *GameStageCreateBillboard(float size, char *texture, float *pos, u16 id)

{
  GfxSprite *sprite;

  sprite = GfxSpriteLayerCreateBillboardByName(g_billboardSpriteLayer,texture);
  sprite->blendMode = (uint)id;
  sprite->billboardMode = 0;
  sprite->flags = sprite->flags | 1;
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
  GfxSpriteSetStencilWrite(sprite,true,0xb2);
  return sprite;
}
