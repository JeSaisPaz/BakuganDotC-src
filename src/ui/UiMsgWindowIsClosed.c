// bdc 0x08816e18 UiMsgWindowIsClosed
#include "bdc.h"

/* Returns whether the message window has finished (state word `+0x0 > 2`). */

bool UiMsgWindowIsClosed(UiMsgWindow *self)

{
  return 2 < self->state;
}

