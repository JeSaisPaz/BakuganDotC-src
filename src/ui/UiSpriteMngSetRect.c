// bdc 0x089ee830 UiSpriteMngSetRect
#include "bdc.h"

/* Sets a sprite's source rectangle (`UiSpriteSetUvRect` with {x, y, w, h}) and size (`UiSpriteSetSize(w,
   h)`). Returns 1 on success, 0 when the slot is empty. `ScriptOpSprite` cmd 5. */

int UiSpriteMngSetRect(UiSpriteMng *self, int slot, int x, int y, int w, int h)

{
  GfxSprite *sprite;
  float rect[4];

  if ((self->layer != (void *)0x0) && (sprite = self->sprites[slot], sprite != (GfxSprite *)0x0)) {
    rect[0] = (float)x;
    rect[1] = (float)y;
    rect[2] = (float)w;
    rect[3] = (float)h;
    UiSpriteSetUvRect(sprite,rect);
    UiSpriteSetSize((float)w,(float)h,sprite);
    return 1;
  }
  return 0;
}
