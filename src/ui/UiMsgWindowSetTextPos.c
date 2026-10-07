// bdc 0x08816504 UiMsgWindowSetTextPos
#include "bdc.h"

/* Sets the text position of the message window (`UiMsgWindowCtor`) (`+0x14` = x, `+0x18` = y; the
   constructor uses the screen centre 240, 136). `UiMsgWindowUpdate` prints the text there (y −
   12). */

void UiMsgWindowSetTextPos(UiMsgWindow *self, s32 x, s32 y)

{
  self->textX = x;
  self->textY = y;
  return;
}

