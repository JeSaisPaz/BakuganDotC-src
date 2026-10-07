// bdc 0x089ec3a0 UiMsgBoxRequestClose
#include "bdc.h"

/* Sets the close-request byte `box+0x59` and returns 0; `UiMsgBoxUpdate` then runs the closing
   states. */

int UiMsgBoxRequestClose(UiMsgBox *self)

{
  self->unk59 = '\x01';
  return 0;
}

