// bdc 0x089a3fb4 UiItemBoxGetMotionName
#include "bdc.h"

/* Copies the name of item-box motion `index` into the 64-byte buffer `out`: 0 `menu_itembox_Open`,
   1 `menu_itembox_Close`, 2..4 `menu_itembox_rotation_L1..L3`, 5..7 `…_R1..R3`. */

void UiItemBoxGetMotionName(u8 index, char *out)
{
    char buf[64];
    const char *names[8];
    int i;

    names[0] = "menu_itembox_Open";
    names[1] = "menu_itembox_Close";
    names[2] = "menu_itembox_rotation_L1";
    names[3] = "menu_itembox_rotation_L2";
    names[4] = "menu_itembox_rotation_L3";
    names[5] = "menu_itembox_rotation_R1";
    names[6] = "menu_itembox_rotation_R2";
    names[7] = "menu_itembox_rotation_R3";
    sprintf(buf, names[index]);
    for (i = 0; i < 64; i++) {
        out[i] = buf[i];
    }
}
