// bdc 0x088cb910 UiTalkBalloonSetText
#include "bdc.h"

/* Sets the message text of the talk balloon (`UiTalkBalloonCtor`) (`+0x58` start and `+0x5c`
   cursor). */

void UiTalkBalloonSetText(UiTalkBalloon *self, char *text)

{
  self->text = text;
  self->textCursor = text;
  return;
}

