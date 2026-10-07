// bdc 0x089f3a78 GfxSpriteCtor
#include "bdc.h"

/* Constructor of a `GfxSprite`: `CoreObjectInit` with no chain, sprite vtable `0x08af583c`,
   default fields (`GfxSpriteInit`) and `+0x124` cleared. */

GfxSprite *GfxSpriteCtor(GfxSprite *sprite)

{
  CoreObjectInit((CoreObject *)sprite,(CoreObject *)0x0);
  sprite->vtable = (void *)&g_gfxSpriteVtbl;
  GfxSpriteInit(sprite);
  sprite->slotFlags = 0;
  return sprite;
}

