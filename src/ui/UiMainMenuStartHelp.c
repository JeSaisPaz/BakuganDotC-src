// bdc 0x089a9564 UiMainMenuStartHelp
#include "bdc.h"

/* Resets the help-dialog state (`+0xe3c`, 4 bytes) and selects help message `msgIndex` (`+0xe3d`)
   for `UiMainMenuRunHelp`. */

void UiMainMenuStartHelp(UiMainMenu *self, u8 msgIndex)

{
  memset(&self->helpStep,0,4);
  self->helpMsg = msgIndex;
  return;
}

