// bdc 0x088c8728 UiTalkBalloonSpriteRelease
#include "bdc.h"

/* Marks a talk balloon sprite (`UiTalkBalloonSprite`: `GfxSprite` subclass, vtable `0x08af2d34`,
   owner window `+0x160`, kind `+0x168`, state `+0x16c`) for deletion (state `+0x16c = 100`) when it
   is a glyph (kind 0); used by `UiTalkBalloonRemoveGlyphs`. */

void UiTalkBalloonSpriteRelease(GfxSprite *sprite)

{
  UiTalkBalloonSprite *s = (UiTalkBalloonSprite *)sprite;

  if (s->kind == 0) {
    s->state = 100;
  }
  return;
}

