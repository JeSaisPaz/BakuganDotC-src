// bdc 0x088c1640 GameFieldCreateLayoutSprite
#include "bdc.h"

/* Creates a sprite on the field's 2D layer `spriteLayer` (`GfxSpriteLayerCreateSpriteByName`) from a `UiLayoutEntry` layout
   record `{x, y, z, u, v, w, h, …, texture}` (z offset by 200), sets its UV rectangle `{u, v, u + w, v + h}`
   and size `w`×`h` (`UiSpriteSetUvRect`, `UiSpriteSetSize`) and, for quad modes 1 and 3, shifts its position
   by half the size. Returns the sprite. */

GfxSprite *GameFieldCreateLayoutSprite(CoreTask *task, const UiLayoutEntry *record)

{
  GfxSprite *sprite;
  s32 quadMode;
  s16 w;
  s16 h;
  float width;
  float height;
  float pos[4] __attribute__((aligned(16)));
  float uv[4];

  pos[0] = (float)record->x;
  pos[1] = (float)record->y;
  pos[2] = (float)(record->z + 200);
  pos[3] = 0.0f;
  sprite = GfxSpriteLayerCreateSpriteByName(((GameFieldTask *)task)->spriteLayer,
                                            record->texture, pos, false);
  uv[0] = (float)record->u;
  uv[1] = (float)record->v;
  uv[2] = uv[0] + (float)record->w;
  uv[3] = uv[1] + (float)record->h;
  w = record->w;
  h = record->h;
  UiSpriteSetUvRect(sprite, uv);
  width = (float)w;
  height = (float)h;
  UiSpriteSetSize(width, height, sprite);
  quadMode = sprite->quadMode;
  if (quadMode < 2) {
    if (quadMode <= 0) {
      return sprite;
    }
  }
  else if (quadMode != 3) {
    return sprite;
  }
  sprite->posX = sprite->posX + width * 0.5f;
  sprite->posY = sprite->posY + height * 0.5f;
  return sprite;
}
