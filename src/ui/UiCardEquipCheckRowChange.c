// bdc 0x0896cbc4 UiCardEquipCheckRowChange
#include "bdc.h"

/* Returns 1 when the input in `UiCardEquip` leaves the current row: Down from the
   bottom card row (or any Down in the large layout) in row 1, or Up in row 2; 0 otherwise. */

int UiCardEquipCheckRowChange(UiCardEquip *self)
{
  int row = self->row;
  if (row > 0) {
    if (row < 2) {
      if (self->bakuganCount < 3) {
        if (((self->base.pad->repeat & 0x40) != 0) && (self->rowCursor[row] >= 2)) {
          return 1;
        }
      } else if ((self->base.pad->repeat & 0x40) != 0) {
        return 1;
      }
    } else if ((row < 3) && ((self->base.pad->repeat & 0x10) != 0)) {
      return 1;
    }
  }
  return 0;
}
