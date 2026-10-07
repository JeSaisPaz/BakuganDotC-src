// bdc 0x089a856c UiMainMenuGetDecision
#include "bdc.h"

/* Returns 0 when Cross (pressed mask bit 0x4000) was not pressed, 1 when it was pressed on an
   unlocked item, 2 on a locked item. */

int UiMainMenuGetDecision(UiMainMenu *self)

{
  if ((((self->base).pad)->pressed & 0x4000) == 0) {
    return 0;
  }
  if (((uint)self->unlockedMask & 1 << ((int)self->cursor & 0x1fU)) == 0) {
    return 2;
  }
  return 1;
}

