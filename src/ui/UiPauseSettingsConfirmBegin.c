// bdc 0x089adfa4 UiPauseSettingsConfirmBegin
#include "bdc.h"

/* Arms the yes/no confirm of `UiPauseSettings`: clears the 4-byte dialog
   state `+0xbbc` (step, message, result, pad) and stores the `DWMesHelp` message index `message` at
   `+0xbbd`; `UiPauseSettingsConfirmStep` then runs the dialog. */

void UiPauseSettingsConfirmBegin(UiPauseSettings *self, u8 message)

{
  memset(&self->dlgStep,0,4);
  self->dlgMessage = message;
  return;
}

