// bdc 0x0883f6a4 BtlResultGetScoreItem
#include "bdc.h"

/* Value of result-screen item `item` (0..0x1e) for `BtlHudRatingResultScreen` and
   `BtlHudArenaResultScreen`: items 0..15 are per-row records (row `item/4`, column `item%4`:
   `BtlResultGetPlayerRecord`; profile word 0x27 + row when profile word 7 is 0, 0xe + row when it
   is 1 or 2, else 0; word 0x1f + row; word 0x23 + row); 0x10 clear bonus, 0x11/0x12/0x13 max combo,
   50, product; 0x14/0x15/0x16 HP%, 20, product; 0x17/0x18/0x19 item pickups, 100, product;
   0x1a/0x1b/0x1c destroyed-building %, 10, product; 0x1d battle score, 0x1e total score; 0 for any
   other item. */

u32 BtlResultGetScoreItem(void *hud, int item)
{
    u32 value = 0;

    if ((u32)item < 0x10) {
        s32 row = item / 4;
        s32 mode;

        switch (item % 4) {
        case 0:
            value = BtlResultGetPlayerRecord(hud, row);
            break;
        case 1:
            mode = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
            if (mode == 0) {
                value = SaveProfileGetWord(SaveGetProfile(), 0x27 + row);
            } else if (mode == 1 || mode == 2) {
                value = SaveProfileGetWord(SaveGetProfile(), 0xe + row);
            }
            break;
        case 2:
            value = SaveProfileGetWord(SaveGetProfile(), 0x1f + row);
            break;
        default:
            value = SaveProfileGetWord(SaveGetProfile(), 0x23 + row);
            break;
        }
        return value;
    }

    switch (item) {
    case 0x10:
        value = BtlResultGetClearBonus(hud);
        break;
    case 0x11:
        value = BtlResultGetMaxCombo(hud);
        break;
    case 0x12:
        value = 50;
        break;
    case 0x13:
        value = BtlResultGetScoreItem(hud, 0x11);
        value = value * BtlResultGetScoreItem(hud, 0x12);
        break;
    case 0x14:
        value = BtlResultGetHpPercent(hud);
        break;
    case 0x15:
        value = 20;
        break;
    case 0x16:
        value = BtlResultGetScoreItem(hud, 0x14);
        value = value * BtlResultGetScoreItem(hud, 0x15);
        break;
    case 0x17:
        value = BtlResultGetItemPickupCount(hud);
        break;
    case 0x18:
        value = 100;
        break;
    case 0x19:
        value = BtlResultGetScoreItem(hud, 0x17);
        value = value * BtlResultGetScoreItem(hud, 0x18);
        break;
    case 0x1a:
        value = BtlResultGetDestroyedBuildingPercent(hud);
        break;
    case 0x1b:
        value = 10;
        break;
    case 0x1c:
        value = BtlResultGetScoreItem(hud, 0x1a);
        value = value * BtlResultGetScoreItem(hud, 0x1b);
        break;
    case 0x1d:
        value = BtlResultGetBattleScore(hud);
        break;
    case 0x1e:
        value = BtlResultGetTotalScore(hud);
        break;
    default:
        break;
    }
    return value;
}
