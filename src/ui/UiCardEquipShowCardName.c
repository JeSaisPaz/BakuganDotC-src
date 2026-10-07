// bdc 0x0896d20c UiCardEquipShowCardName
#include "bdc.h"

/* Shows (`show` = 1) the name of the card under the cursor of `UiCardEquip`
   (`UiCardEquipPrintCardName`, alpha 1) or hides it (alpha 0, also for empty slots); marks the
   name dirty. */

void UiCardEquipShowCardName(UiCardEquip *self, u8 show)

{
  if (show == '\0') {
    self->nameAlpha = 0.0f;
  }
  else if (self->cardIds[self->selBakugan * 4 + self->rowCursor[self->row]] == 0xff) {
    self->nameAlpha = 0.0f;
  }
  else {
    UiCardEquipPrintCardName
              (self,self->cardIds[self->selBakugan * 4 + self->rowCursor[self->row]]);
    self->nameAlpha = 1.0f;
  }
  self->nameDirty = '\x01';
  return;
}

