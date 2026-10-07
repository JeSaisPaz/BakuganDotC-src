// bdc 0x089f4c54 GfxSpriteFlipU
#include "bdc.h"

/* Mirrors the sprite horizontally by swapping u0 and u1 in its four vertices (vertex 0/2 and 1/3
   pairs). Does nothing when `vertices` is NULL. */

void GfxSpriteFlipU(GfxSprite *sprite)
{
  GfxSpriteVertex *v = sprite->vertices;

  if (v != NULL) {
    GfxSpriteVertex *v1 = v + 1;
    GfxSpriteVertex *v2 = v + 2;
    GfxSpriteVertex *v3 = v + 3;

    v->u = v1->u;
    v1->u = v2->u;
    v2->u = v->u;
    v3->u = v1->u;
  }
}
