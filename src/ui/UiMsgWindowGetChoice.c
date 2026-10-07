// bdc 0x08816e70 UiMsgWindowGetChoice
#include "bdc.h"

/* Returns the message window's choice (`+0x20`): always once the window is closed, while open only
   if a choice has been made (byte `+0x44`), else -1. The save tasks treat 0 as 'yes' (open the
   delete-list dialog to make room). */

s32 UiMsgWindowGetChoice(UiMsgWindow *self)
{
  s32 choice = -1;

  if (!UiMsgWindowIsClosed(self)) {
    if (self->hasChoice != 0) {
      choice = self->choice;
    }
    return choice;
  }
  return self->choice;
}
