// bdc 0x088ce988 UiFieldHudCreateHiddenSprite
#include "bdc.h"

/* Creates a sprite with texture `name` (`GfxFindTexture`) on the layer `layer` (`GfxSpriteLayerCreateSprite`)
   at the origin, hides it (clears flag bit 0) and returns it. */

GfxSprite *UiFieldHudCreateHiddenSprite(char *name, void *layer)

{
  void *texture;
  GfxSprite *sprite;
  float pos[4] __attribute__((aligned(16)));

  texture = GfxFindTexture(name);
  pos[2] = 0.0f;
  pos[1] = 0.0f;
  pos[0] = 0.0f;
  pos[3] = 0.0f;
  sprite = GfxSpriteLayerCreateSprite(layer, texture, pos, false);
  sprite->flags = sprite->flags & 0xfffffffe;
  return sprite;
}
