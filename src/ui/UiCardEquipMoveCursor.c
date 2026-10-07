// bdc 0x0896c320 UiCardEquipMoveCursor
#include "bdc.h"

/* Handles D-pad repeat input for the cursor row of `UiCardEquip`: row 0 wraps the
   tab cursor with Up/Down; row 1 moves inside the 2x2 (small layout) or 1x4 card grid of the
   selected Bakugan. Returns 1 when the cursor moved, 0 otherwise (other rows, no direction, or
   the move would leave the grid). Repeat bits: 0x10 Up, 0x20 Right, 0x40 Down, 0x80 Left. */

int UiCardEquipMoveCursor(UiCardEquip *self)
{
    int row = self->row;
    PadState *pad;
    s8 cur;

    if (row < 1) {
        if (row < 0)
            return 0;
        /* row 0: tab cursor, wraps around groups[3] count */
        pad = self->base.pad;
        if (pad->repeat & 0x10) {
            if (self->rowCursor[row] == 0)
                self->rowCursor[row] = (s8)self->groups[3][1] - 1;
            else
                self->rowCursor[row] = self->rowCursor[row] - 1;
            return 1;
        }
        if (pad->repeat & 0x40) {
            if (self->rowCursor[row] == (s8)self->groups[3][1] - 1)
                self->rowCursor[row] = 0;
            else
                self->rowCursor[row] = self->rowCursor[row] + 1;
            return 1;
        }
        return 0;
    }
    if (row >= 2)
        return 0;

    pad = self->base.pad;
    if (self->bakuganCount < 3) {
        /* 2x2 grid */
        if (pad->repeat & 0x10) {
            cur = self->rowCursor[row];
            if (cur < 2)
                return 0;
            self->rowCursor[row] = cur - 2;
            return 1;
        }
        if (pad->repeat & 0x40) {
            cur = self->rowCursor[row];
            if (cur >= 2)
                return 0;
            self->rowCursor[row] = cur + 2;
            return 1;
        }
        if (pad->repeat & 0x80) {
            cur = self->rowCursor[row];
            if (cur % 2 != 1)
                return 0;
            self->rowCursor[row] = cur - 1;
            return 1;
        }
        if (pad->repeat & 0x20) {
            cur = self->rowCursor[row];
            if (cur % 2 != 0)
                return 0;
            self->rowCursor[row] = cur + 1;
            return 1;
        }
        return 0;
    }

    /* 1x4 row */
    if (pad->repeat & 0x80) {
        cur = self->rowCursor[row];
        if (cur % 4 == 0)
            return 0;
        self->rowCursor[row] = cur - 1;
        return 1;
    }
    if (pad->repeat & 0x20) {
        cur = self->rowCursor[row];
        if (cur % 4 == 3)
            return 0;
        self->rowCursor[row] = cur + 1;
        return 1;
    }
    return 0;
}
