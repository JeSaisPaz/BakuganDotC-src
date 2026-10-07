// bdc 0x08971c34 UiOptionCheckConfirm
#include "bdc.h"

/* Returns 1 when Cross is pressed (repeat mask) while a button row (OK/Defaults) of
   `UiOption` is selected. */

int UiOptionCheckConfirm(UiOption *self)

{
  if (('\x03' < (char)self->cursor) && ((((self->base).pad)->repeat & 0x4000) != 0)) {
    return 1;
  }
  return 0;
}

