// bdc 0x08840640 BtlResultShowScoreItem
#include "bdc.h"

/* Shows result item `item` (0..0x1e) on screen: when `value` is -999 it is
   fetched with BtlResultGetScoreItem first; then the number is written with
   BtlResultSetDigits into the rating result sprites (items 0x10..0x16 and
   0x1a..0x1e) or the arena result sprites (items 0..0xf). Items 0x17..0x19
   and out-of-range items are not shown. */
void BtlResultShowScoreItem(BtlHud *hud, int item, int value)
{
    if (value == -999) {
        value = BtlResultGetScoreItem(hud, item);
    }
    if ((u32)item >= 0x1f) {
        return;
    }
    switch (item) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x1a:
    case 0x1b:
    case 0x1c:
    case 0x1d:
    case 0x1e:
        BtlResultSetDigits(hud, hud->ratingSprites, value, item);
        break;
    case 0x17:
    case 0x18:
    case 0x19:
        break;
    default:
        BtlResultSetDigits(hud, hud->arenaSprites, value, item);
        break;
    }
}
