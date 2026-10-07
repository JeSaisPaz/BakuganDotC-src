// bdc 0x089ad514 UiPauseSettingsCancelPressed
#include "bdc.h"

/* Returns 1 when Circle was pressed (pad `+5` bit 0x20). */

int UiPauseSettingsCancelPressed(UiPauseSettings *self)

{
  if ((((self->base).pad)->pressed & 0x2000) != 0) {
    return 1;
  }
  return 0;
}

