// bdc 0x08840494 BtlResultSetDigits
#include "bdc.h"

/* Writes number `value` into the digit sprites of result item `item`: copies the whole
   `g_btlResultDigitSprites` table (31 rows x 8) to the stack, reverses row `item` (up to its -1
   terminator) so the least significant digit comes first, and for each digit sprite sets its cell
   to the decimal digit (column digit / 5, row digit % 5 of the font sheet), showing it (flag bit 0)
   unless the remaining value is 0 past the first digit (leading zeros hidden). `hud` is unused. */

void BtlResultSetDigits(void *hud, GfxSprite **sprites, int value, int item)
{
    s32 table[31][8];
    s32 reversed[8];
    s32 count;
    s32 i;
    s32 k;

    (void)hud;
    memcpy(table, g_btlResultDigitSprites, sizeof(table));
    count = 0;
    for (i = 0; i < 8; i++) {
        reversed[i] = -1;
        if (table[item][i] == -1) {
            break;
        }
        count++;
    }
    k = count;
    for (i = 0; i < 8; i++) {
        k--;
        if (k >= 0) {
            reversed[k] = table[item][i];
        }
    }
    for (i = 0; i < 8; i++) {
        s32 index = reversed[i];
        s32 digit;
        GfxSprite *sprite;
        bool visible;

        if (index == -1) {
            return;
        }
        visible = true;
        digit = value % 10;
        GfxSpriteSetCell(sprites[index], (float)(digit / 5), (float)(digit % 5));
        sprite = sprites[index];
        if (value == 0 && i > 0) {
            visible = false;
        }
        if (visible) {
            sprite->flags |= 1;
        } else {
            sprite->flags &= ~1u;
        }
        value = value / 10;
    }
}
