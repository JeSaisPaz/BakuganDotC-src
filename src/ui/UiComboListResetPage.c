// bdc 0x089b3268 UiComboListResetPage
#include "bdc.h"

/* Resets the page state of `UiComboList`: `+0x90 = 0`, `+0x94 = 1`. */

void UiComboListResetPage(UiComboList *self)

{
  self->attrIcon[0] = 0;
  self->attrIcon[1] = 1;
  return;
}

