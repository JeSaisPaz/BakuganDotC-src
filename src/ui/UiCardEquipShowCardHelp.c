// bdc 0x0896d5d0 UiCardEquipShowCardHelp
#include "bdc.h"

/* Shows (`show` = 1) the help text of the card under the cursor of `UiCardEquip`
   (`UiCardEquipPrintCardHelp`) or hides it; marks the help dirty. */

void UiCardEquipShowCardHelp(UiCardEquip *self, u8 show)

{
  if (show == '\0') {
    self->helpAlpha = 0.0f;
  }
  else if (self->cardIds[self->selBakugan * 4 + self->rowCursor[self->row]] == 0xff) {
    self->helpAlpha = 0.0f;
  }
  else {
    UiCardEquipPrintCardHelp
              (self,self->cardIds[self->selBakugan * 4 + self->rowCursor[self->row]]);
    self->helpAlpha = 1.0f;
  }
  self->helpDirty = '\x01';
  return;
}

