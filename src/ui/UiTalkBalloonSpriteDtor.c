// bdc 0x088c7aa8 UiTalkBalloonSpriteDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of a talk balloon sprite (`UiTalkBalloonSprite`: `GfxSprite`
   subclass, vtable `0x08af2d34`, owner window `+0x160`, kind `+0x168`, state `+0x16c`): resets the
   vtable, runs the base sprite destructor `GfxSpriteDtor` and frees it when `flags & 1`. */

void UiTalkBalloonSpriteDtor(GfxSprite *sprite, u32 flags)

{
  if (sprite != (GfxSprite *)0x0) {
    sprite->vtable = g_uiTalkBalloonSpriteVtbl;
    GfxSpriteDtor(sprite,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(sprite,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

