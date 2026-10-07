// bdc 0x08948518 UiBattleRecordSetListFrame
#include "bdc.h"

/* Sets `sprite` to the list-frame region of its texture (UV rect 72, 95, 328×115), size 328×115
   at (72, 95), and returns it. */

GfxSprite *UiBattleRecordSetListFrame(GfxSprite *sprite)
{
  float rect[4];

  rect[0] = 72.0f;
  rect[1] = 95.0f;
  rect[2] = 328.0f;
  rect[3] = 115.0f;
  GfxSpriteSetUvRectXYWH(sprite, rect);
  UiSpriteSetSize(328.0f, 115.0f, sprite);
  sprite->posX = 72.0f;
  sprite->posY = 95.0f;
  return sprite;
}
