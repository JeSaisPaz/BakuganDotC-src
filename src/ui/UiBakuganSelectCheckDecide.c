// bdc 0x0892f364 UiBakuganSelectCheckDecide
#include "bdc.h"

/* On CROSS (pad `pressed` bit 0x4000) returns 1 when the entry under the cursor is owned and not
   locked by the pair mask `+0x1cec`, 2 otherwise; 0 without input. */

int UiBakuganSelectCheckDecide(UiBakuganSelect *self)
{
  u32 id;
  int result;

  if ((self->base.pad->pressed & 0x4000) == 0) {
    return 0;
  }
  id = self->entries[self->cursor].bakugan;
  result = 2;
  if (id != 0) {
    result = 1;
    if ((self->unselectableMask[0] & (1 << (id & 0x1f))) != 0) {
      result = 2;
    }
  }
  return result;
}
