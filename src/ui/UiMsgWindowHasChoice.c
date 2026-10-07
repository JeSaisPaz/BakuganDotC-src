// bdc 0x08816e34 UiMsgWindowHasChoice
#include "bdc.h"

/* Returns whether the player has made a choice in the message window (`UiMsgWindowCtor`) (byte
   `+0x44`). See `UiMsgWindowGetChoice`. */

bool UiMsgWindowHasChoice(UiMsgWindow *self)

{
  return self->hasChoice != '\0';
}

