// bdc 0x089483d4 UiBattleRecordCreateIconSprite
#include "bdc.h"

/* Creates a hidden 32×32 sprite on `layer` showing the whole texture `texName`
   (`GfxFindTexture`, `GfxSpriteLayerCreateSprite`, UV rect 0,0,32,32, `UiSpriteSetSize`) and
   returns it. */

GfxSprite *UiBattleRecordCreateIconSprite(const char *texName, void *layer)

{
  void *texture;
  GfxSprite *sprite;
  float pos[4] __attribute__((aligned(16)));
  float uv[4] __attribute__((aligned(16)));

  texture = GfxFindTexture(texName);
  pos[2] = 0.0f;
  pos[1] = 0.0f;
  pos[0] = 0.0f;
  pos[3] = 0.0f;
  sprite = GfxSpriteLayerCreateSprite(layer, texture, pos, false);
  uv[0] = 0.0f;
  uv[1] = 0.0f;
  uv[2] = 32.0f;
  uv[3] = 32.0f;
  GfxSpriteSetUvRectXYWH(sprite, uv);
  UiSpriteSetSize(32.0f, 32.0f, sprite);
  sprite->flags = sprite->flags & 0xfffffffe;
  return sprite;
}
