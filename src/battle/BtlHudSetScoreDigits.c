// bdc 0x08833bf4 BtlHudSetScoreDigits
#include "bdc.h"

/* Draws `value` with the up-to-3 digit sprites of score-board counter `counter`: the sprite
   indices come from a stack copy of the 14x3 table `g_btlHudScoreDigitSprites` (-1 ends a row).
   Each digit `d`, least significant first, selects cell `(d / 5, d % 5)` of the digit sheet
   (`GfxSpriteSetCell`) and is shown, except that digits after the first are hidden once the
   remaining value is 0 (leading zeros). `value` -1 hides the whole counter. */
void BtlHudSetScoreDigits(BtlHud *self, GfxSprite **sprites, s32 value, s32 counter)
{
    s32 table[14][3];
    s32 *row;
    s32 digit;
    s32 i;
    GfxSprite *sprite;

    (void)self;
    memcpy(table, g_btlHudScoreDigitSprites, sizeof(table));
    row = table[counter];
    if (value == -1) {
        for (i = 0; i < 3; i++) {
            if (row[i] == -1) {
                return;
            }
            sprites[row[i]]->flags &= ~1u;
        }
        return;
    }
    for (i = 0; i < 3; i++) {
        if (row[i] == -1) {
            return;
        }
        digit = value % 10;
        GfxSpriteSetCell(sprites[row[i]], (float)(digit / 5), (float)(digit % 5));
        sprite = sprites[row[i]];
        if (value == 0 && i > 0) {
            sprite->flags &= ~1u;
        } else {
            sprite->flags |= 1u;
        }
        value = value / 10;
    }
}
