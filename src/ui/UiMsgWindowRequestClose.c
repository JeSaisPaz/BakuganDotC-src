// bdc 0x08816e08 UiMsgWindowRequestClose
#include "bdc.h"

/* Sets the close-request byte `+0x1c` of the message window (`UiMsgWindowCtor`), so
   `UiMsgWindowUpdate` leaves the waiting state on the next frame without input. Returns 0. Used
   by `BtlMainUpdateQuitPrompt` and the net lobby when the session state changes under an open
   prompt. */

s32 UiMsgWindowRequestClose(UiMsgWindow *self)

{
  self->closeRequested = '\x01';
  return 0;
}

