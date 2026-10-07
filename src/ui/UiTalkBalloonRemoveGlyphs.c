// bdc 0x088c97fc UiTalkBalloonRemoveGlyphs
#include "bdc.h"

/* Walks the text printer's sprite list (`printer(+0x10)->+0x1c`) of the talk balloon
   (`UiTalkBalloonCtor`) and marks each glyph for removal (`UiTalkBalloonSpriteRelease`): all of
   them when `all` is set, otherwise only those whose byte `+0x180` is clear. */

void UiTalkBalloonRemoveGlyphs(UiTalkBalloon *self, bool all)

{
  GfxSprite *sprite;
  
  sprite = (self->printer->layer).head;
  while (sprite != (GfxSprite *)0x0) {
    if (all) {
      UiTalkBalloonSpriteRelease(sprite);
      sprite = sprite->next;
    }
    else if (((UiTalkBalloonSprite *)sprite)->keep == 0) {
      UiTalkBalloonSpriteRelease(sprite);
      sprite = sprite->next;
    }
    else {
      sprite = sprite->next;
    }
  }
  return;
}

