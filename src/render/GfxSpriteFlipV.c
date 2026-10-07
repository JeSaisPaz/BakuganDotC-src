// bdc 0x089f4c88 GfxSpriteFlipV
#include "bdc.h"

/* Mirrors the sprite vertically by swapping v0 and v1 in its four vertices (vertex 0/1 and 2/3
   pairs). Does nothing when `vertices` is NULL. */

void GfxSpriteFlipV(GfxSprite *sprite)
{
  GfxSpriteVertex *v = sprite->vertices;

  if (v != NULL) {
    GfxSpriteVertex *v1 = v + 1;
    GfxSpriteVertex *v2 = v + 2;
    GfxSpriteVertex *v3 = v + 3;

    v->v = v2->v;
    v2->v = v1->v;
    v1->v = v->v;
    v3->v = v2->v;
  }
}
