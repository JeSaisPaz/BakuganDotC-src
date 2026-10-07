// bdc 0x089eea44 UiSpriteMngOffsetUv
#include "bdc.h"

/* Scrolls the UV rectangle of sprite-manager slot `slot` by (a, b, c, d) texels with
   `UiSpriteAddUvRect`. Returns 1 on success, 0 otherwise. */

int UiSpriteMngOffsetUv(UiSpriteMng *self, int slot, int a, int b, int c, int d)

{
  int ok;
  float delta[4];
  
  ok = 0;
  if ((self->layer != (void *)0x0) && (self->sprites[slot] != (GfxSprite *)0x0)) {
    delta[0] = (float)a;
    delta[1] = (float)b;
    delta[2] = (float)c;
    delta[3] = (float)d;
    UiSpriteAddUvRect(self->sprites[slot],delta);
    ok = 1;
  }
  return ok;
}

