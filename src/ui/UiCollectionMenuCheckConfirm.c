// bdc 0x08975894 UiCollectionMenuCheckConfirm
#include "bdc.h"

/* Returns 0 unless Cross was pressed in `UiCollectionMenu`; then 1 when the
   selected entry is enabled in its page mask (`entryMask` on page 0, `subMasks[selMain]` on the
   sub-page) and 2 when it is not. */

int UiCollectionMenuCheckConfirm(UiCollectionMenu *self)

{
  u32 bit;
  u8 mask;

  if ((((self->base).pad)->pressed & 0x4000) == 0) {
    return 0;
  }
  bit = 1 << (&self->selMain)[self->page];
  if (self->page == 0) {
    mask = self->entryMask;
  } else {
    mask = self->subMasks[self->selMain];
  }
  if ((mask & bit) == 0) {
    return 2;
  }
  return 1;
}
