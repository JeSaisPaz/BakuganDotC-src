// bdc 0x089ad354 UiPauseSettingsHandleUpDown
#include "bdc.h"

/* Moves the cursor vertically (pad repeat bit 0x10 up, 0x40 down) through rows 0..3 and the button
   row (4), wrapping; returns 1 when it moved, 0 otherwise. */

int UiPauseSettingsHandleUpDown(UiPauseSettings *self)
{
    PadState *pad = self->base.pad;
    s32 cur;
    s8 next;

    if ((pad->repeat & 0x10) != 0) {
        cur = self->cursor;
        next = 4;
        if (cur != 0) {
            next = 3;
            if (cur < 4) {
                next = cur - 1;
            }
        }
        self->cursor = next;
        return 1;
    }
    if ((pad->repeat & 0x40) != 0) {
        cur = self->cursor;
        next = 0;
        if (cur != self->itemCount - 1) {
            next = 0;
            if (cur < 4) {
                next = cur + 1;
            }
        }
        self->cursor = next;
        return 1;
    }
    return 0;
}
