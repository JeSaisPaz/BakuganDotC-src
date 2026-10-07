// bdc 0x088c9868 UiTalkBalloonClearText
#include "bdc.h"

/* Clears the current page of the talk balloon (`UiTalkBalloonCtor`): removes all glyphs
   (`UiTalkBalloonRemoveGlyphs`), sets the icon states (frame `+0x130` → 2, `+0x12c` → 0,
   cursor `+0x134` → 3), clears `+0x79` and sets the step state `+0x48 = 4`. */

void UiTalkBalloonClearText(UiTalkBalloon *self)

{
  UiTalkBalloonRemoveGlyphs(self,true);
  if (self->frameSprite != (GfxSprite *)0x0) {
    ((UiTalkBalloonSprite *)self->frameSprite)->state = 2;
  }
  if (self->iconSprite != (GfxSprite *)0x0) {
    ((UiTalkBalloonSprite *)self->iconSprite)->state = 0;
  }
  if (self->cursorSprite != (GfxSprite *)0x0) {
    ((UiTalkBalloonSprite *)self->cursorSprite)->state = 3;
  }
  self->pageFlags[1] = '\0';
  self->state = 4;
  return;
}

