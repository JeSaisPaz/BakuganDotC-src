// bdc 0x08819f14 UiSpriteCreate
#include "bdc.h"

/* Creates a sprite for `texture` on `layer` (`GfxSpriteLayerCreateSprite(layer, texture, {0,0,200.0,0}, 0)`),
   sets its UV rect to (x, y, x+w, y+h) with `UiSpriteSetUvRect` and size (w, h) with
   `UiSpriteSetSize`; when the sprite's `quadMode` (+0xe4) is 1 or 3 the position is shifted by
   half the size (centre anchor). Returns the sprite, or NULL when layer or texture is NULL.
   Called by `UiSpriteMngAdd` via the manager's layer. */

void *UiSpriteCreate(void *layer, void *texture, int x, int y, int w, int h)
{
  GfxSprite *sprite;
  int mode;
  float fw;
  float fh;
  float uv[4];
  float pos[4] __attribute__((aligned(16)));

  sprite = NULL;
  if (layer == NULL || texture == NULL)
    return sprite;

  pos[0] = 0.0f;
  pos[1] = 0.0f;
  pos[2] = 200.0f;
  pos[3] = 0.0f;
  sprite = GfxSpriteLayerCreateSprite(layer, texture, pos, false);

  uv[0] = (float)x;
  uv[1] = (float)y;
  uv[2] = (float)(x + w);
  uv[3] = (float)(h + y);
  UiSpriteSetUvRect(sprite, uv);

  fw = (float)w;
  fh = (float)h;
  UiSpriteSetSize(fw, fh, sprite);

  mode = sprite->quadMode;
  if (mode < 2) {
    if (mode <= 0)
      return sprite;
  } else if (mode != 3) {
    return sprite;
  }
  sprite->posX = sprite->posX + fw * 0.5f;
  sprite->posY = sprite->posY + fh * 0.5f;
  return sprite;
}
