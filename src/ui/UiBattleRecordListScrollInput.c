// bdc 0x089495b4 UiBattleRecordListScrollInput
#include "bdc.h"

/* Scroll input of the record lists of `UiBattleRecord` (held d-pad, pad
   `buttons`): Up when the top row `+0x78` > 0 sets direction `+0x7c` = 1, Down when it is < 15 (20
   Bakugan, 5 visible) sets 2, both with SE 1, and returns 1; otherwise clears the direction and
   returns 0. */

s32 UiBattleRecordListScrollInput(UiBattleRecord *self)
{
    PadState *pad = self->base.pad;

    if ((pad->buttons & 0x10) && self->scrollTop > 0) {
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        self->scrollDir = 1;
        return 1;
    }
    if ((pad->buttons & 0x40) && self->scrollTop < 0xf) {
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        self->scrollDir = 2;
        return 1;
    }
    self->scrollDir = 0;
    return 0;
}
