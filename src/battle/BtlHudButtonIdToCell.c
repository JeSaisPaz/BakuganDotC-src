// bdc 0x08835bac BtlHudButtonIdToCell
#include "bdc.h"

/* Maps a button-guide button id (1..9) to its cell row in the button icon sheet: 1→2, 2→5,
   3→0, 4→3, 5→1, 6→8, 7→9, 8→7, 9→4; 0 and any other id → 0. `self` is unused. */
s32 BtlHudButtonIdToCell(BtlHud *self, s32 buttonId)
{
    (void)self;
    switch (buttonId) {
    case 1:
        return 2;
    case 2:
        return 5;
    case 3:
        return 0;
    case 4:
        return 3;
    case 5:
        return 1;
    case 6:
        return 8;
    case 7:
        return 9;
    case 8:
        return 7;
    case 9:
        return 4;
    default:
        return 0;
    }
}
