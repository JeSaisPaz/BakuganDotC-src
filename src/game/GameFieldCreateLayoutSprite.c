// bdc 0x088c1640 GameFieldCreateLayoutSprite
#include "bdc.h"

/* Creates a sprite on the field's 2D layer `spriteLayer` (`GfxSpriteLayerCreateSpriteByName`) from an `s16` layout record
   `{x, y, z, u, v, w, h, …, texture name at +0x10}` (z offset by 200), sets its UV rectangle `{u, v, u + w, v + h}`
   and size `w`×`h` (`UiSpriteSetUvRect`, `UiSpriteSetSize`) and, for quad modes 1 and 3, shifts its position
   by half the size. Returns the sprite. */

GfxSprite *GameFieldCreateLayoutSprite(CoreTask *task, s16 *record)

{
  GfxSprite *sprite;
  s32 quadMode;
  s16 w;
  s16 h;
  float width;
  float height;
  float pos[4] __attribute__((aligned(16)));
  float uv[4];

  pos[0] = (float)record[0];
  pos[1] = (float)record[1];
  pos[2] = (float)(record[2] + 200);
  pos[3] = 0.0f;
  sprite = GfxSpriteLayerCreateSpriteByName(((GameFieldTask *)task)->spriteLayer,
                                            *(const char **)(record + 8), pos, false);
  uv[0] = (float)record[3];
  uv[1] = (float)record[4];
  uv[2] = uv[0] + (float)record[5];
  uv[3] = uv[1] + (float)record[6];
  w = record[5];
  h = record[6];
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
