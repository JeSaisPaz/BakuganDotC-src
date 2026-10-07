// bdc 0x089a5610 UiButtonCellOffset
#include "bdc.h"

/* Returns `base` plus 1 when `button` is 9, else `base` (a 10-entry u16 table with only entry 9
   set). Callers use it to pick a sprite cell; the meaning of button 9 is not established. */

int UiButtonCellOffset(u8 button, u16 base)
{
    u16 table[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};

    return (u32)table[button] + (u32)base;
}
