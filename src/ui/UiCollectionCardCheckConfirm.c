// bdc 0x08984bf8 UiCollectionCardCheckConfirm
#include "bdc.h"

/* Returns 0 unless Cross was pressed in `UiCollectionCard`; then 1 when the
   selected slot holds an owned card and 2 when it is empty. */

int UiCollectionCardCheckConfirm(UiCollectionCard *self)
{
    if ((self->base.pad->pressed & 0x4000) != 0) {
        return self->slots[self->cursor + self->page * 4] != 0xff ? 1 : 2;
    }
    return 0;
}
