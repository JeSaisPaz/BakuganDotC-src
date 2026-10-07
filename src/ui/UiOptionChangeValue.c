// bdc 0x0897192c UiOptionChangeValue
#include "bdc.h"

/* Left/Right on a value row of `UiOption`: decrements (pad bit 0x80) or increments
   (pad bit 0x20) `values[cursor]` within `0..valueCounts[cursor]-1` and records the direction in
   `arrowSide` (0 = left, 1 = right). Row 3 is locked while its value is -1. Returns 1 when the
   value changed, else 0 (also for cursor rows outside 0..3). */

int UiOptionChangeValue(UiOption *self)
{
    s8 row = (s8)self->cursor;
    PadState *pad;

    if (row < 0 || row >= 4) {
        return 0;
    }
    if (row == 3 && self->values[3] == -1) {
        return 0;
    }
    pad = self->base.pad;
    if (pad->buttons & 0x80) {
        if (self->values[row] > 0) {
            self->values[row]--;
            self->arrowSide = 0;
            return 1;
        }
    } else if (pad->buttons & 0x20) {
        if (self->values[row] < (s8)self->valueCounts[row] - 1) {
            self->values[row]++;
            self->arrowSide = 1;
            return 1;
        }
    }
    return 0;
}
