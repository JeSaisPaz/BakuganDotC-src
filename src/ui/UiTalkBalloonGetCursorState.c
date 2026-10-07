// bdc 0x088cc1bc UiTalkBalloonGetCursorState
#include "bdc.h"

/* Returns the state `+0x16c` of the cursor icon `+0x134` of the talk balloon
   (`UiTalkBalloonCtor`), or 0 without one; the frame sprite and the fading icon use it to follow
   the page state. */

s32 UiTalkBalloonGetCursorState(UiTalkBalloon *self)

{
  if (self->cursorSprite != (GfxSprite *)0x0) {
    return ((UiTalkBalloonSprite *)self->cursorSprite)->state;
  }
  return 0;
}

