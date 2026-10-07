// bdc 0x0882f144 BtlHudSetTimerDigits
#include "bdc.h"

/* Writes the battle timer into the battle HUD's digit sprites (self->sprites): `seconds`, clamped
   to 0..999, as hundreds/tens/units into sprites 3, 4, 5, and `subsec` * 1.66666663f, truncated
   and clamped to 0..99, as tens/units into sprites 7, 8. Each digit d selects sprite cell column
   d / 5, row d % 5 (GfxSpriteSetCell). */

void BtlHudSetTimerDigits(BtlHud *self, s32 seconds, s32 subsec)
{
    s32 hundredths;
    s32 digit;

    if (seconds < 0) {
        seconds = 0;
    }
    if (seconds > 999) {
        seconds = 999;
    }
    hundredths = (s32)((float)subsec * 1.66666663f);
    if (hundredths < 0) {
        hundredths = 0;
    }
    if (hundredths > 99) {
        hundredths = 99;
    }
    digit = (seconds / 100) % 10;
    GfxSpriteSetCell(self->sprites[3], (float)(digit / 5), (float)(digit % 5));
    digit = ((seconds % 100) / 10) % 10;
    GfxSpriteSetCell(self->sprites[4], (float)(digit / 5), (float)(digit % 5));
    digit = ((seconds % 100) % 10) % 10;
    GfxSpriteSetCell(self->sprites[5], (float)(digit / 5), (float)(digit % 5));
    digit = (hundredths / 10) % 10;
    GfxSpriteSetCell(self->sprites[7], (float)(digit / 5), (float)(digit % 5));
    digit = (hundredths % 10) % 10;
    GfxSpriteSetCell(self->sprites[8], (float)(digit / 5), (float)(digit % 5));
}
