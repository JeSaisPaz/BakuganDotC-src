// bdc 0x089ec3cc UiMsgBoxHasOwner
#include "bdc.h"

/* Returns whether the message box has its owner window object (`+0x0`). */

bool UiMsgBoxHasOwner(UiMsgBox *self)

{
  return self->owner != (void *)0x0;
}

