// bdc 0x089ad46c UiPauseSettingsButtonDecided
#include "bdc.h"

/* Returns 1 when Cross was pressed (pad `+5` bit 0x40) while the cursor is on the button row. */

int UiPauseSettingsButtonDecided(UiPauseSettings *self)

{
  if (('\x03' < self->cursor) && ((((self->base).pad)->pressed & 0x4000) != 0)) {
    return 1;
  }
  return 0;
}

