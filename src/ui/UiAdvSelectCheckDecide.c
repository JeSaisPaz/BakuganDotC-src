// bdc 0x0891a750 UiAdvSelectCheckDecide
#include "bdc.h"

/* Returns 0 unless CROSS was pressed (pad byte `+5` bit 0x40); then 1 when the selected candidate
   is unlocked, 2 when it is locked. */

int UiAdvSelectCheckDecide(UiAdvSelect *self)

{
  int result;
  
  if ((((self->base).pad)->pressed & 0x4000) != 0) {
    result = 2;
    if (self->candidates[self->cursor].locked == '\0') {
      result = 1;
    }
    return result;
  }
  return 0;
}

